#ifndef UTILITIES_SINGLETON_HXX_
#define UTILITIES_SINGLETON_HXX_

#include <memory>
#include <functional>

template<class T>
class Singleton {
public:
    using singleton_t = Singleton<T>;
    template<class... A>
    static std::shared_ptr<T> get(A&&... a) {
        return getInternal([&a...]{
            return new T(std::forward<A>(a)...);
        });
    }
private:
    static std::shared_ptr<T> getInternal(std::function<T*()> cb) {
        static auto inst = std::shared_ptr<T>();
        if(!inst) {
            inst = std::shared_ptr<T>(cb());
        }
        return inst;
    }
};



#endif /* UTILITIES_SINGLETON_HXX_ */
