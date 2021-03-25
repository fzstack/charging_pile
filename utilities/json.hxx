#ifndef UTILITIES_JSON_HXX_
#define UTILITIES_JSON_HXX_

#include <rtthread.h>
#include <cJSON.h>
#include <string_view>
#include <string>
#include <memory>
#include <stdexcept>
#include <initializer_list>
#include <any>
#include <variant>
#include "json_iterator.hxx"

template<class T>
class JsonWrapper;

class Json;
class JsonIterator;

namespace std {
string to_string(Json value);
}

class Json {
    friend std::string std::to_string(Json value);
    friend class JsonIterator;
public:

    enum class Type {
        Null, Number, Boolean, String, Object, Array,
    };

    Json();
    Json(std::string_view value);
    Json(std::string value);
    Json(const char* value);
    Json(bool value);
    Json(int value);
    Json(float value);
    Json(nullptr_t);
    Json(const Json& other);
    Json(Json&& other);
    Json(std::initializer_list<Json> obj);
private:
    Json(std::shared_ptr<cJSON> root, cJSON* self, cJSON* parent);

    enum class CtorType {
        FromString,
        FromArray,
    };
    template<CtorType T>
    class private_ctor{};

    Json(private_ctor<CtorType::FromString>, std::string_view value);
    Json(private_ctor<CtorType::FromArray>, std::initializer_list<Json> obj);
    //Json(JsonWrapper<void> wrapper);
public:
    ~Json();
public:
    static Json parse(std::string_view value);
    static Json array(std::initializer_list<Json> obj);

public:
    JsonIterator begin();
    JsonIterator end();
    void push_back(Json&& item);
    Type getType() const;
    Json operator[](std::string_view itemName) const;
    Json operator[](const char* itemName) const;
    Json operator[](const char* itemName);
    Json operator[](int index) const;

//    template<class T>
//    T operator[](JsonWrapper<T> itemName) {
//        return (*this)[itemName.get()];
//    }

    template<class T>
    T operator[](JsonWrapper<T> itemName) const {
        return (*this)[itemName.get()];
    }

    operator int() const;
    operator float() const;
    operator std::string() const;
    operator bool() const;
    operator nullptr_t() const;

    void operator=(int value);
    void operator=(std::string_view value);
    void operator=(const char* value);
    void operator=(bool value);
    void operator=(nullptr_t value);
    Json& operator=(Json&& other);
    Json& operator=(const Json& other);

private:
    void reset(int newType = cJSON_NULL); //重置为Null对象
    void moveNode(cJSON* dest, cJSON* source);
    void toArray(std::initializer_list<Json> obj);
    Json getItem(const char* itemName) const;
    Json getOrCreateItem(const char* itemName);
    static char* copyStr(char* str);

private:
    struct Extra {
        cJSON *self = nullptr, *parent = nullptr;
    };

public:
    std::shared_ptr<cJSON> root;
private:
    std::shared_ptr<Extra> extra;
    bool moving = false;
};

class json_error: public std::runtime_error {
    using std::runtime_error::runtime_error;
};

class json_type_error: public json_error {
    using json_error::json_error;
};

class json_item_error: public json_error {
    using json_error::json_error;
};

class json_item_not_found: public json_item_error {
    using json_item_error::json_item_error;
};

template<class T>
class JsonWrapper {
public:
    JsonWrapper(std::string_view itemName): itemName(itemName) {}
    std::string_view get() {
        return itemName;
    }
private:
    std::string_view itemName;
};

namespace json_literals {
JsonWrapper<int> operator "" _i( const char *col, size_t n);
JsonWrapper<std::string> operator "" _s( const char *col, size_t n);
JsonWrapper<bool> operator "" _b( const char *col, size_t n);
JsonWrapper<nullptr_t> operator "" _n( const char *col, size_t n);
JsonWrapper<void> operator "" _a( const char *col, size_t n);
}



#endif /* UTILITIES_JSON_HXX_ */
