#ifndef UTILITIES_IDX_HXX_
#define UTILITIES_IDX_HXX_

#include <map>
#include <list>
#include <memory>
#include <functional>
#include "err.hxx"
#include <rtthread.h>
#include "thread_local.hxx"
#include <algorithm>
#include <boost/pfr.hpp>
#include "signals.hxx"
#include <queue>
#include <unordered_map>

#define LOG_MEM_STATE(...) rt_kprintf(##__VA_ARGS__)

//NOTE: 目前阻止部分交叠申请, 因为那需要保证buffer连续
template <class Owner, class Addr, Addr Null, Addr Max>
class IdxAsset {
public:
    IdxAsset(Addr addr = Null): addr(addr) {};
    IdxAsset(std::nullptr_t n): addr(Null) {};

    void* getPtr(std::size_t size, std::function<std::list<void*>()> getIdxFieldPtrs);
    std::size_t& getRefCount(std::size_t size, std::function<std::list<void*>()> getIdxFieldPtrs);
    bool isAccessed() const;
    std::shared_ptr<rt_uint8_t[]> dispose(std::size_t size);

    bool isRoot() {
        //对于每个ownerSpecs的owner的每个current，判断this是否落在current内
        auto& f = field[this];
        auto& owner = ownerSpecs[f.owner];
        auto& range = owner.range;
        auto& data = owner.data;
        auto first = range.begin();
        auto second = first;
        ++second;
        for(; second != range.end(); ++first, ++second) {
            auto found = data.find(*first);
            if(found == data.end()) continue;
            auto current = found->second.current.get();
            auto size = *second - *first;
            //判断this是否落在current中
            if((rt_uint8_t*)current <= (rt_uint8_t*)this && (rt_uint8_t*)this < ((rt_uint8_t*)current + size)) {
                return false;
            }
        }
        return true;
    }

    struct Field {
        std::shared_ptr<Owner> owner = Owner::get();
        std::shared_ptr<rt_uint8_t[]> current = nullptr;
        bool isRoot;
        std::size_t size;
        std::function<std::list<void*>()> getIdxFieldPtrs;
    };

    struct OwnerSpec {
        std::list<Addr> range = {0, Max + 1};
        std::list<std::size_t> count = {0};

        struct Data {
            std::shared_ptr<rt_uint8_t[]> current = nullptr;
            std::shared_ptr<rt_uint8_t[]> backup = nullptr;
        };

        std::unordered_map<Addr, Data> data = {};
    };

    std::shared_ptr<rt_uint8_t[]> createNode(std::shared_ptr<Owner> owner, std::size_t size) {
        auto ownerSpecKvp = ownerSpecs.find(owner);
        if(ownerSpecKvp == ownerSpecs.end()) {
            ownerSpecs[owner] = {};
        }

        auto& ownerSpec = ownerSpecs[owner];

        auto& range = ownerSpec.range;
        auto& count = ownerSpec.count;

        //没找到则创建
        //裂开range、count链表
        auto front = std::lower_bound(range.begin(), range.end(), addr);
        auto rightAddr = addr + size;
        if(*front != addr) {
            //需要裂开, 但是得判断与原有的有没有交叉，有的话抛异常

            if(rightAddr > *front) throw not_implemented{"cross detected"};

            //判断是否落在引用计数大于0的范围内
            auto countFront = count.begin();
            std::advance(countFront, std::distance(range.begin(), front) - 1);
            if(*countFront != 0) throw not_implemented{"cross detected"};
            ++countFront;

            if(rightAddr == *front) {
                //右边不需要裂开
                range.insert(front, addr);
                count.insert(countFront, 1);
            } else {
                //右边需要裂开
                range.insert(front, addr);
                count.insert(countFront, 1);
                range.insert(front, rightAddr);
                count.insert(countFront, 0);
            }

        } else {
            //左边不需要裂开, 如果不需要裂开, 且计数 > 0, 需要特殊处理
            auto back = front;
            ++back;
            if(rightAddr > *back) throw not_implemented{"cross detected"};
            auto countback = count.begin();
            std::advance(countback, std::distance(range.begin(), back) - 1);
            if(*countback != 0) throw not_implemented{"cross detected"};

            ++*countback; //自身计数+1

            ++countback;

            if(rightAddr != *back) {
                //右边需要裂开
                range.insert(back, rightAddr);
                count.insert(countback, 0);
            }
        }

        //rt_kprintf("%08x after ^@%02x, range: ", this, addr);
        //printContainer(range);
        //rt_kprintf(", count: ");
        //printContainer(count);
        //rt_kprintf("\n");

        auto data = std::shared_ptr<rt_uint8_t[]>(new rt_uint8_t[size]);
        auto backup = std::shared_ptr<rt_uint8_t[]>(new rt_uint8_t[size]);

        owner->read(addr, data.get(), size);
        memcpy(backup.get(), data.get(), size);

        auto& curData = ownerSpec.data[addr];

        curData.current = data;
        curData.backup = backup;

        return data;
    }

    template<class U>
    static void printContainer(U& t) {
        rt_kprintf("{");
        for(const auto& val: t) {
            rt_kprintf("%02x, ", val);
        }
        rt_kprintf("}");
    }


    static std::unordered_map<void*, Field> field;
    static std::unordered_map<std::shared_ptr<Owner>, OwnerSpec> ownerSpecs;

protected:
    Addr addr;
};

template <class T, class Owner, class Addr, Addr Null, Addr Max>
class Idx: private IdxAsset<Owner, Addr, Null, Max> { //NOTE: 改变指针要调用dispose
    using asset_t = IdxAsset<Owner, Addr, Null, Max>;
public:
    using asset_t::asset_t;
    Idx(const Idx<T, Owner, Addr, Null, Max>& other) {
        asset_t::addr = other.addr;
        if(other.isAccessed()) {
            getPtr();
        }
    }

    ~Idx() {
        //rt_kprintf("dispose %08x due to destrcuct\n", this);
        dispose();
        asset_t::field.erase(this);
    }

    T* operator->() { return (T*)getPtr(); }
    T& operator*() { return *(T*)getPtr(); }

    template<class... Args>
    auto operator()(Args&&... args) {
        //new(getPtr())T(std::forward<Args>(args)...);
        T::create(*(T*)getPtr(), std::forward<Args>(args)...);
        return *this;
    }

    auto operator()() {
        *(T*)getPtr() = T{};
        return *this;
    }

    Addr get() {
        return asset_t::addr;
    }

    void operator=(Addr addr) {
        //rt_kprintf("dispose %08x due to copy Addr(%02x)\n", this, addr);
        if(asset_t::addr == addr) return;
        dispose(); this->addr = addr;
    }
    void operator=(const Idx<T, Owner, Addr, Null, Max>& other) {
        if(asset_t::addr == other.addr) return;
        //rt_kprintf("dispose %08x due to copy other(%02x)\n", this, other.addr);
        dispose();
        asset_t::addr = other.addr;
        if(other.isAccessed()) {
            //rt_kprintf("%08x@%04x copy and inst from %08x %s\n", this, asset_t::addr, &other, typeid(T).name());
            getPtr();
        }
    }

    void operator=(Idx<T, Owner, Addr, Null, Max>&& other) {
        if(asset_t::addr == other.addr) return;
        //rt_kprintf("dispose %08x due to move other(%02x)\n", this, other.addr);
        dispose();
        asset_t::addr = other.addr;
        if(other.isAccessed()) {
            //rt_kprintf("%08x@%04x copy and inst from %08x %s\n", this, asset_t::addr, &other, typeid(T).name());
            getPtr();
        }
    }

    void operator=(const std::nullptr_t&) { dispose(); asset_t::addr = Null; }

    bool operator==(const std::nullptr_t& n) { return asset_t::addr == Null; }
    bool operator==(const Idx<T, Owner, Addr, Null, Max>& other) { return asset_t::addr == other.addr; }

    bool operator!=(const std::nullptr_t& n) { return !(*this == n); }
    bool operator!=(const Idx<T, Owner, Addr, Null, Max>& other) { return !(*this == other); }

private:
    template<class IT> struct is_idx: public std::false_type { using type = void; };
    template<class IT> struct is_idx<Idx<IT, Owner, Addr, Null, Max>>: public std::true_type { using type = IT; };
    template<class IT> static constexpr bool is_idx_v = is_idx<IT>::value;
    template<class IT> using is_idx_t = typename is_idx<IT>::type;

    std::list<void*> getIdxFieldPtrs() {
        if(!asset_t::isAccessed())
            return std::list<void*>{};
        auto ptr = getPtr();
        auto idxList = std::list<void*>{};
        boost::pfr::for_each_field(*(T*)ptr, [&](auto& field, std::size_t idx) {
            if(is_idx_v<std::decay_t<decltype(field)>>) {
                idxList.push_back((void*)&field);
            }
        });
        return idxList;
    }

    void* getPtr() {
        return asset_t::getPtr(sizeof(T), signal(&Idx<T, Owner, Addr, Null, Max>::getIdxFieldPtrs, this));
    }

    std::size_t& getRefCount() {
        return asset_t::getRefCount(sizeof(T), signal(&Idx<T, Owner, Addr, Null, Max>::getIdxFieldPtrs, this));
    }

    void dispose() {
        auto obj = asset_t::dispose(sizeof(T));
        if(obj) {
            auto& f = asset_t::field[this];
            ((T*)obj.get())->~T();
            if(f.isRoot) {
                //rt_kprintf("!!!!!!!!!!!!!!!!!!!ROOT(%02x) DISPOSED!!!!!!!!!!!!!!!!!!!\n", asset_t::addr);
                auto marking = std::queue<void*>{};
                auto visited = std::unordered_map<void*, bool>{};

                //1.获取现在所有的root
                visited[this] = true;
                for(const auto& fi: asset_t::field) {
                    if(fi.first == this || !fi.second.isRoot) continue;
                    marking.push(fi.first);
                    //rt_kprintf("find root: %08x@%02x\n", fi.first, *(Addr*)fi.first);
                }

                while(!marking.empty()) {
                    auto idx = marking.front();
                    marking.pop();
                    if(visited[idx]) continue;

                    auto found = asset_t::field.find(idx);
                    if(found != asset_t::field.end()) {
                        auto& fi = found->second;
                        for(const auto& child: fi.getIdxFieldPtrs()) {
                            marking.push(child);
                        }
                    }
                    visited[idx] = true;
                }

                //2.删除所有未标记的节点
                for(const auto& fi: asset_t::field) {
                    auto found = visited.find(fi.first);
                    if(found != visited.end()) continue;
                    //rt_kprintf("%08x@%02x need to dispose\n", fi.first, *(rt_uint16_t*)fi.first);
                    ((asset_t*)fi.first)->dispose(fi.second.size);
                    asset_t::field.erase(fi.first);
                }
            }
        }
    }
};

template <class Owner, class Addr, Addr Null, Addr Max>
std::unordered_map<void*, typename IdxAsset<Owner, Addr, Null, Max>::Field> IdxAsset<Owner, Addr, Null, Max>::field = {};

template <class Owner, class Addr, Addr Null, Addr Max>
std::unordered_map<std::shared_ptr<Owner>, typename IdxAsset<Owner, Addr, Null, Max>::OwnerSpec> IdxAsset<Owner, Addr, Null, Max>::ownerSpecs = {};

template <class Owner, class Addr, Addr Null, Addr Max>
void* IdxAsset<Owner, Addr, Null, Max>::getPtr(std::size_t size, std::function<std::list<void*>()> getIdxFieldPtrs) {
    if(field.find(this) == field.end()) {
        field[this] = {};
        field[this].isRoot = isRoot();
        field[this].getIdxFieldPtrs = getIdxFieldPtrs;
        field[this].size = size;
    }
    auto& f = field[this];
    auto owner = f.owner;
    auto& current = f.current;

    //TODO: 判断是否已求值

    if(addr == Null) throw std::out_of_range{"nullptr"};
    if(addr + size - 1 > Max) throw std::out_of_range{"out of addr space"};
    if(!owner) throw std::runtime_error{"ctx not provided"};

    if(!current) { //未访问过成员 TODO: 判断current是否已失效
        //1. 判断地址是否已存在, 即判断addr
        auto ownerSpecKvp = ownerSpecs.find(owner);
        if(ownerSpecKvp == ownerSpecs.end()) {
            ownerSpecs[owner] = {};
        }

        auto& ownerSpec = ownerSpecs[owner];
        auto& range = ownerSpec.range;
        //auto& count = ownerSpec.count;

        auto dataKvp = ownerSpec.data.find(addr);
        if(dataKvp != ownerSpec.data.end()) {
            auto& data = dataKvp->second;
            //如果找到, 则需要校验, 判断size是否满足要求
            auto foundIt = std::find(range.begin(), range.end(), addr);
            auto dist = std::distance(range.begin(), foundIt);
            auto nextIt = foundIt;
            ++nextIt;
            auto rangeSize = std::size_t(*nextIt - addr);
            if(rangeSize != size) throw std::runtime_error{"range size mismatch"};
            auto countIt = ownerSpec.count.begin();
            std::advance(countIt, dist);
            auto& countVal = *countIt;
            if(countVal <= 0) throw std::runtime_error{"range count error"};
            current = data.current;
            ++countVal;

            //rt_kprintf("%08x after +@%02x, range: ", this, addr);
            //printContainer(range);
            //rt_kprintf(", count: ");
            //printContainer(count);
            //rt_kprintf("\n");
            return (void*)current.get();
        }
        current = createNode(owner, size);
        return (void*)current.get();
    } else {
        auto& ownerSpec = ownerSpecs[owner];
        auto& range = ownerSpec.range;
        auto foundIt = std::find(range.begin(), range.end(), addr);
        auto nextIt = foundIt;
        nextIt++;
        if(foundIt == range.end() || nextIt == range.end() || std::size_t(*nextIt - *foundIt) != size) {
            current = createNode(owner, size);
        }
    }

    return (void*)current.get();
}

template <class Owner, class Addr, Addr Null, Addr Max>
std::size_t& IdxAsset<Owner, Addr, Null, Max>::getRefCount(std::size_t size, std::function<std::list<void*>()> getIdxFieldPtrs) {
    if(field.find(this) == field.end()) {
        field[this] = {};
        field[this].isRoot = isRoot();
        field[this].getIdxFieldPtrs = getIdxFieldPtrs;
        field[this].size = size;
    }
    auto& f = field[this];
    auto owner = f.owner;

    auto ownerSpecKvp = ownerSpecs.find(owner);
    if(ownerSpecKvp == ownerSpecs.end()) {
        throw std::runtime_error{"not prepared"};
    }

    auto& ownerSpec = ownerSpecs[owner];
    auto& range = ownerSpec.range;

    auto dataKvp = ownerSpec.data.find(addr);
    if(dataKvp == ownerSpec.data.end()) {
        throw std::runtime_error{"not prepared"};
    }

    auto foundIt = std::find(range.begin(), range.end(), addr);
    auto dist = std::distance(range.begin(), foundIt);
    auto nextIt = foundIt;
    ++nextIt;
    auto rangeSize = std::size_t(*nextIt - addr);
    if(rangeSize != size) throw std::runtime_error{"range size mismatch"};
    auto countIt = ownerSpec.count.begin();
    std::advance(countIt, dist);
    auto& countVal = *countIt;
    if(countVal <= 0) throw std::runtime_error{"range count error"};
    return countVal;
}

template <class Owner, class Addr, Addr Null, Addr Max>
bool IdxAsset<Owner, Addr, Null, Max>::isAccessed() const {
    if(field.find((void*)this) == field.end()) {
        return false;
    }
    return field[(void*)this].current != nullptr;
}

template <class Owner, class Addr, Addr Null, Addr Max>
std::shared_ptr<rt_uint8_t[]> IdxAsset<Owner, Addr, Null, Max>::dispose(std::size_t size) {
    if(field.find(this) == field.end()) return nullptr;

    auto& f = field[this];
    auto owner = f.owner;
    auto& current = f.current;

    if(!current) {
        //rt_kprintf("%08x current is null\n", this);
        return nullptr;
    }

    //rt_kprintf("%08x current is not null\n", this);

    auto& ownerSpec = ownerSpecs[owner];

    //减少引用计数、合并未引用的range
    auto& range = ownerSpec.range;
    auto& count = ownerSpec.count;

    auto found = std::find(range.begin(), range.end(), addr);

    if(found == range.end()) {
        //rt_kprintf("%08x @%02x, W already disposed", this, addr);
        return nullptr;
    }

    auto countIt = count.begin();
    std::advance(countIt, std::distance(range.begin(), found));
    auto beforeIt = countIt;
    --beforeIt;

    auto afterIt = countIt;
    ++afterIt;

    auto afterRangeIt = found;
    ++afterRangeIt;

    if(*countIt == 0 || std::size_t(*afterRangeIt - *found) != size) {
        //rt_kprintf("%08x @%02x, W already disposed", this, addr);
        return nullptr;
    }
    --*countIt;
    if(*countIt <= 0) {
        //准备合并
        if(*afterIt == 0) {
            //rt_kprintf("erase range value %d\n", *afterRangeIt);
            //rt_kprintf("erase count @%d\n", std::distance(count.begin(), afterIt));
            range.erase(afterRangeIt);
            count.erase(afterIt);
        }

        if(countIt != count.begin()) {

            if(*beforeIt == 0) {
                auto beforeRangeIt = found;
                //rt_kprintf("erase range value %d\n", *beforeRangeIt);
                //rt_kprintf("erase count @%d\n", std::distance(count.begin(), countIt));
                range.erase(beforeRangeIt);
                count.erase(countIt);
            }
        }

        //判断是否满足写回条件
        auto& data = ownerSpec.data[addr];
        if(memcmp(data.current.get(), data.backup.get(), size)) {
            owner->write(addr, data.current.get(), size);
        }

        auto result = data.current;
        ownerSpec.data.erase(addr);

        //rt_kprintf("%08x after =@%02x, range: ", this, addr);
        //printContainer(range);
        //rt_kprintf(", count: ");
        //printContainer(count);
        //rt_kprintf("\n");

        return result;
    }

    //rt_kprintf("%08x after -@%02x, range: ", this, addr);
    //printContainer(range);
    //rt_kprintf(", count: ");
    //printContainer(count);
    //rt_kprintf("\n");

    current = nullptr;
    return nullptr;
}

#endif /* UTILITIES_IDXBASE_HXX_ */
