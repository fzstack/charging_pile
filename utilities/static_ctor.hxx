#pragma once

template <class T>
struct StaticCtor {
protected:
    using static_ctor_t = StaticCtor<T>;
private:
    struct Helper {
        Helper() {
            T::staticCtor();
        }
    };

protected:
    StaticCtor() {
        static Helper helper;
    }
};
