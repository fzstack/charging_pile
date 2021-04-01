#ifndef COMPONENTS_PERSISTENT_STORAGE_HXX_
#define COMPONENTS_PERSISTENT_STORAGE_HXX_

#include <memory>
#include <rtthread.h>
#include <utilities/thread_local.hxx>
#include <stdexcept>
#include <utilities/idx.hxx>
#include <array>
#include <Mutex.h>
#include <utilities/tiny_type_id.hxx>

extern "C" {
#include <at24cxx.h>
}

//用法: 使用make返回智能指针，然后自己存储对象的弱指针
//所有智能指针都指向内存的同一个对象，当一个指针指针引用计数归零时，在e²prom中保存数据

//比如
//auto config = ps->make<Config>();
//NOTE: 如果e²prom不存在对应的对象的数据，则调用对象的默认构造器（比如默认参数就可以这样实现）
//config->cnt++;
//然后config的智能指针被析构之后，把数据保存到e²prom中

class PersistentStorage: public std::enable_shared_from_this<PersistentStorage> {
    template <class T>
    class IdxSess;
    template <class T> friend class IdxSess;
public:
    PersistentStorage(at24cxx_device_t device, int size);

private:

    class IdxOwner {
    public:
        IdxOwner(std::shared_ptr<PersistentStorage> outer): outer(outer) {

        }

        void read(rt_uint16_t addr, rt_uint8_t* data, std::size_t size) {
            at24cxx_read(outer->device, addr, data, size);
            //memcpy(data, &buffer[addr], size);
//            rt_kprintf("\033[32mread from %04x, size %d: ", addr, size);
//            for(auto i = 0u; i < size; i++) {
//                rt_kprintf("%02x ", data[i]);
//            }
//            rt_kprintf("\n\033[0m");
        }

        void write(rt_uint16_t addr, rt_uint8_t* data, std::size_t size) {
            rt_kprintf("\033[34mwrite to %04x, size %d: ", addr, size);
            for(auto i = 0u; i < size; i++) {
                rt_kprintf("%02x ", data[i]);
            }
            rt_kprintf("\n\033[0m");
            at24cxx_write(outer->device, addr, data, size);
            //memcpy(&buffer[addr], data, size);
        }

        static std::shared_ptr<IdxOwner> get() {
            return *owner;
        }

        static ThreadLocal<std::shared_ptr<IdxOwner>> owner;

    private:
        std::shared_ptr<PersistentStorage> outer;
        //std::array<rt_uint8_t, 2048> buffer;
    };

    friend class IdxOwner;

    template<class T>
    using Idx = ::Idx<T, IdxOwner, rt_uint16_t, 0xffff, 2048 - 1>;

    struct HeapNode {
        static void create(HeapNode& node, rt_uint16_t size) {
            node = HeapNode{};
            node.size = size;
        }

        rt_uint16_t size;
        ///node没有被析构，导致prev和next没有被析构
        Idx<HeapNode> prev;
        Idx<HeapNode> next;
    };

    template<class T> struct List;

    template<class T>
    struct ListIterator {
        ListIterator(List<T> list, Idx<T> node, bool isEnd): list(list), node(node), isEnd(isEnd) {

        }

        ListIterator<T>& operator++() {
            if(!isEnd) {
                node = node->next;
                if(node == list.getFront()) {
                    isEnd = true;
                }

            }
            return *this;
        }

        Idx<T> operator*() const {
            if(isEnd) throw not_implemented{"meet end"};
            return node;
        }

        bool operator!=(const ListIterator<T>& rhs) {
            if(isEnd != rhs.isEnd) return true;
            if(isEnd == true && rhs.isEnd == true) return false;
            return node != rhs.node;
        }

        bool operator==(const ListIterator<T>& rhs) {
            return !(*this != rhs);
        }

        List<T> list;
        Idx<T> node;
        bool isEnd = false;
    };

    template<class T>
    struct List {
        void add(Idx<T> node) {
            if(front == nullptr) {
                front = node;
                //TODO: 下面两次赋值会导致bug发生
                ///因为node->next持有了node自身的Idx, 导致引用计数 + 2
                node->next = node;
                node->prev = node;
            } else {
                auto rear = front->prev;
                rear->next = node;
                node->prev = rear;
                front->prev = node;
                node->next = front;
            }
        }

        void remove(Idx<T> node) {
            if(front == node) {
                if(front == front->next) { //仅剩一个节点
                    front = nullptr;
                } else {
                    front = front->next;
                }
            }
            auto nodePrev = node->prev;
            auto nodeNext = node->next;
            nodePrev->next = nodeNext;
            nodeNext->prev = nodePrev;
        }

        ListIterator<T> begin() {
            return {*this, front, front == nullptr};
        }

        ListIterator<T> end() {
            return {*this, front, true};
        }

        Idx<T> getFront() {
            return front;
        }

    private:
        Idx<T> front = nullptr;
    };

    struct ObjNode {
        Idx<ObjNode> prev;
        Idx<ObjNode> next;
    };

    struct TypeNode { //元数据节点

        static void create(TypeNode& node, std::size_t hash) {
            node = TypeNode{};
            node.hash = hash;
        }

        std::size_t hash; //类型哈希
        List<ObjNode> obj;
        //TODO: field链表首元节点
        Idx<TypeNode> prev;
        Idx<TypeNode> next;
    };


    struct Meta { //元数据头, 放在eeprom开头
        //Meta(std::size_t deviceSize);
        static void create(Meta& self, std::size_t deviceSize);
        static Idx<Meta> get();
        static Idx<Meta> make(std::size_t deviceSize);

        bool isValid() {
            return magic == TypeId<Meta>::get();
        }

        std::size_t magic = TypeId<Meta>::get(); //魔数，判断是否需要格式化
        List<HeapNode> idle = {};
        List<HeapNode> alloc = {};
        List<TypeNode> type = {};
    };

private:
    struct MakeResult {
        bool created;
        rt_uint16_t addr;
    };

public:
    void format();

    void test() {
        auto guard = getCtxGuard();
        //format();
        auto data = std::array<rt_uint8_t, 114>{};
        owner->read(0, &data[0], 114);
    }

    template<class T>
    Idx<T> make() {
        auto guard = getCtxGuard();
        auto result = makeInternal(TypeId<T>::get(), sizeof(T));
        auto idx = Idx<T>{result.addr};
        if(!result.created) {
            idx();
        }
        *idx;
        return idx;
    }

private:
    MakeResult makeInternal(std::size_t hash,std::size_t size);

    rt_uint16_t alloc(std::size_t size);
    void free(rt_uint16_t addr);

    std::shared_ptr<void> getCtxGuard();

private:
    std::shared_ptr<IdxOwner> owner = nullptr;
    std::weak_ptr<void> holder = {};
    at24cxx_device_t device;
    const int size;
    static const char* kMutex;
    rtthread::Mutex mutex;
};

//#include <utilities/singleton.hxx>
//namespace Preset {
//class PersistentStorage: public Singleton<PersistentStorage>, public ::PersistentStorage {
//    friend class Singleton<PersistentStorage>;
//    PersistentStorage(): ::PersistentStorage(at24cxx_init(kI2c, kAddr), kSize) {}
//    static const char* kI2c;
//    static const int kAddr;
//    static const int kSize;
//};
//}

//#include <utilities/singleton.hxx>
//#include "persistent_storage_fake.hxx"
//namespace Preset {
//class PersistentStorage: public Singleton<PersistentStorage>, public ::PersistentStorageFake {
//    friend class Singleton<PersistentStorage>;
//    PersistentStorage(): ::PersistentStorageFake() {}
//};
//}
//
namespace std {
    template<class T>
    struct iterator_traits<typename PersistentStorage::ListIterator<T>> {
        using iterator_category = input_iterator_tag;
    };
}


#endif /* COMPONENTS_PERSISTENT_STORAGE_HXX_ */
