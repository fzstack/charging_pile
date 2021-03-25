#ifndef UTILITIES_CMD_UTILITIES_HXX_
#define UTILITIES_CMD_UTILITIES_HXX_

#include <rtthread.h>
#include <functional>
#include <type_traits>
#include "nested.hxx"
#include <stdlib.h>
#include <string>
#include <unordered_map>
#include <deque>

#define ASSERT_MIN_NARGS(n) \
    do { \
        if(argc < n) { \
            LOG_E("too few args"); \
            return; \
        } \
    } while(0)\

#define ASSERT_ARG(name, cond) \
    do { \
        if(!(cond)) { \
            LOG_E("bad " #name " arg"); \
            return; \
        } \
    } while(0) \

class Cmd {
public:
    Cmd(int argc, char** argv);
    void operator()(std::function<void(Cmd& cmd)> cb);

public:
    template<class T>
    auto get(const char* name = nullptr) -> std::enable_if_t<std::is_integral_v<T> || std::is_enum_v<T>, T> {
        return (T)atoi(getFrom(name));
    }

    template<class T>
    auto get(const char* name = nullptr) -> std::enable_if_t<std::is_floating_point_v<T>, T> {
        return (T)atof(getFrom(name));
    }

    template<class T>
    auto get(const char* name = nullptr) -> std::enable_if_t<std::is_same_v<char*, T>, T> {
        return getFrom(name);
    }

    template<class T>
    auto get(const char* name = nullptr) -> std::enable_if_t<std::is_same_v<std::string, T>, T> {
        return getFrom(name);
    }

    template<class T>
    auto select(std::unordered_map<T, std::function<void()>>&& sel) {
        select(nullptr, std::move(sel));
    }

    template<class T>
    auto select(const char* name, std::unordered_map<T, std::function<void()>>&& sel) {
        auto key = get<T>(name);
        auto found = sel.find(key);
        assert(found != sel.end(), [&](){
            auto valid = std::string{};
            auto size = sel.size();
            auto count = 0u;
            for(const auto& kvp: sel) {
                valid += getStr(kvp.first);
                if(count + 1 < size) {
                    valid += "|";
                }
                count++;
            }
            return "bad arg " + key + ", valid are: [" + valid + "]";
        });
        found->second();
    }

    void assert(bool predict, std::string msg);
    void assert(bool predict, std::function<std::string()> gen);

private:
    char* getFromPos();
    char* getFromName(const char* name);
    char* getFrom(const char* name);

    template<class T>
    auto getStr(T& t) -> std::enable_if_t<std::is_same_v<std::decay_t<T>, std::string>, std::string> {
        return t;
    }

    template<class T>
    auto getStr(T& t) -> std::enable_if_t<std::is_arithmetic_v<std::decay_t<T>>, std::string> {
        return to_string(t);
    }

private:
    std::deque<char*> q;
    std::unordered_map<std::string, char*> m;
};


#endif /* APPLICATIONS2_CMD_UTILITIES_HXX_ */
