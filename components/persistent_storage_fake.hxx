#pragma once

#include <memory>
#include <map>

class PersistentStorageFake {
public:
    template<class T>
    std::shared_ptr<T> make() {
        auto hash = typeid(T).hash_code();
        auto found = m.find(hash);
        if(found == m.end()) {
            auto p = std::make_shared<T>();
            m.insert({hash, p});
            return p;
        } else {
            return std::reinterpret_pointer_cast<T>(found->second);
        }
    }

private:
    std::map<std::size_t, std::shared_ptr<void>> m;
};


