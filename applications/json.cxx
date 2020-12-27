/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-19     imgcr       the first version
 */
#include "json.hxx"
#include <cJSON_util.h>
#include <string>
#include <utilities/err.hxx>
#include <utilities/signals.hxx>
#include <variant>
#include <map>

#define LOG_TAG "app.json"
#define LOG_LVL LOG_LVL_DBG
#include <ulog.h>

using namespace std;
using namespace std::string_literals;

Json::Json(): root(shared_ptr<cJSON>(cJSON_CreateObject(), cJSON_Delete)), extra(make_shared<Extra>()) {
    extra->self = root.get();
    if(!extra->self) throw json_error{"json create failed"};
}

Json::Json(string_view value): root(shared_ptr<cJSON>(cJSON_CreateString(value.data()), cJSON_Delete)), extra(make_shared<Extra>()) {
    extra->self = root.get();
    if(!extra->self) throw json_error{"json create failed"};
}

Json::Json(const char* value): Json(string_view(value)) {

}

Json::Json(bool value): root(shared_ptr<cJSON>(cJSON_CreateBool(value), cJSON_Delete)), extra(make_shared<Extra>()) {
    extra->self = root.get();
    if(!extra->self) throw json_error{"json create failed"};
}

Json::Json(int value): root(shared_ptr<cJSON>(cJSON_CreateNumber(value), cJSON_Delete)), extra(make_shared<Extra>()) {
    extra->self = root.get();
    if(!extra->self) throw json_error{"json create failed"};
}

Json::Json(nullptr_t): root(shared_ptr<cJSON>(cJSON_CreateNull(), cJSON_Delete)), extra(make_shared<Extra>()){
    extra->self = root.get();
    if(!extra->self) throw json_error{"json create failed"};
}

Json::~Json() {

}


//拷贝构造器是拷贝引用
//拷贝赋值是深拷贝
Json::Json(const Json& other): root(other.root), extra(make_shared<Extra>()) {
    extra->parent = other.extra->parent;
    extra->self = other.extra->self;
}

//TODO: 完善move语义
Json::Json(Json&& other): root(other.root), extra(make_shared<Extra>()) {
    extra->parent = other.extra->parent;
    extra->self = other.extra->self;
}

Json::Json(initializer_list<Json> obj): Json() {
    //auto pWrap = get_if<Json>(&obj);

    //如果元素数不为2, 则肯定是数组
    //如果元素数量为2且
    auto first = obj.begin();
    if(obj.size() != 2 || first->getType() != Type::String) {

        auto allObj = true;
        for(const auto& child: obj) {
            if(child.getType() != Type::Object) {
                allObj = false;
                break;
            }
        }

        if(allObj) {
            for(auto& child: obj) {
                if(child.extra->self->next != nullptr) throw not_implemented{"not a kvp"};

                //pOtherRoot是根
                auto childName = child.extra->self->child->string;
                getOrCreateItem(childName) = move(child.getItem(childName));
            }
        } else {
            //数组
            throw not_implemented{"TODO array"};
        }


    } else {
        //对象键值对
        auto last = obj.begin() + 1;
        //if(!last) throw json_cast_error{"value not valid json"}; //TODO: 字面量支持
        //(*(Json*)this)[(string)*pKey] = move(*last);
        auto keyName = first->extra->self->valuestring;
        getOrCreateItem(keyName) = move(Json(*last));
    }
}

Json::Json(shared_ptr<cJSON> root, cJSON* self, cJSON* parent): root(root), extra(make_shared<Extra>()) {
    extra->parent = parent;
    extra->self = self;
}

Json::Json(private_ctor, string_view value): root(shared_ptr<cJSON>(cJSON_Parse(value.data()), cJSON_Delete)), extra(make_shared<Extra>()) {
    extra->self = root.get();
}

Json Json::parse(string_view value) {
    auto inst = Json(private_ctor{}, value);
    return inst;
}

Json::Type Json::getType() const {
    return map<int, Type> {
        {cJSON_NULL, Type::Null},
        {cJSON_Number, Type::Number},
        {cJSON_True, Type::Boolean},
        {cJSON_False, Type::Boolean},
        {cJSON_String, Type::String},
        {cJSON_Object, Type::Object},
        {cJSON_Array, Type::Array},
    }.find(extra->self->type)->second;
}

Json Json::operator[](string_view itemName) const {
    return (*this)[itemName.data()];
}

Json Json::operator[](const char* itemName) const {
    return getItem(itemName);
}

Json Json::operator[](const char* itemName) {
    return getOrCreateItem(itemName);
}

Json::operator int() const {
    if(extra->self->type != cJSON_Number)
        throw json_cast_error{"not a number"};
    return extra->self->valueint;
}

Json::operator string() const {
    if(extra->self->type != cJSON_String)
        throw json_cast_error{"not a string"};
    return extra->self->valuestring;
}

Json::operator bool() const {
    if(extra->self->type != cJSON_True && extra->self->type != cJSON_False)
        throw json_cast_error{"not a boolean"};
    return extra->self->type == cJSON_True;
}

Json::operator nullptr_t() const {
    if(extra->self->type != cJSON_NULL)
        throw json_cast_error{"not null"};
    return nullptr;
}

void Json::operator=(int value) {
    if(extra->self->type != cJSON_Number) reset(cJSON_Number);
    cJSON_SetIntValue(extra->self, value);
}

void Json::operator=(string_view value) {
    reset(cJSON_String);
    extra->self->valuestring = new char[value.size() + 1];
    value.copy(extra->self->valuestring, value.size());
    extra->self->valuestring[value.size()] = '\0';
}

void Json::operator=(const char* value) {
    *this = string_view(value);
}

void Json::operator=(bool value) {
    auto nextType = value ? cJSON_True : cJSON_False;
    if(extra->self->type != nextType) reset(nextType);
}

void Json::operator=(nullptr_t value) {
    if(extra->self->type != cJSON_NULL) reset(cJSON_NULL);
}

Json& Json::operator=(Json&& _other) {
    auto self = extra->self;
    auto other = _other.extra->self;

    self->type = other->type;
    other->type = cJSON_NULL;

    cJSON_Delete(self->child);
    self->child = other->child;
    other->child = nullptr;

    if(self->valuestring != nullptr)
        cJSON_free(self->valuestring);
    self->valuestring = other->valuestring;
    other->valuestring = nullptr;

    self->valuedouble = other->valuedouble;
    other->valuedouble = 0;

    self->valueint = other->valueint;
    other->valueint = 0;
    return *this;
}

Json& Json::operator=(const Json& other) {
    //TODO: 拷贝复制
    //LOG_W("json copy op");
    auto copied = cJSON_Duplicate(other.extra->self, true);
    reset(cJSON_NULL);
    auto name = extra->self->string;
    *extra->self = *copied;
    extra->self->string = name;
    cJSON_free(copied);
    return *this;
}

void Json::reset(int newType) {
    auto c = extra->self;

    if (!(c->type&cJSON_IsReference) && c->child) {
        cJSON_Delete(c->child);
        c->child = nullptr;
    }
    if (!(c->type&cJSON_IsReference) && c->valuestring) {
        cJSON_free(c->valuestring);
        c->valuestring = nullptr;
    }
    c->type = newType;
}

void Json::updateRootWith(cJSON* rootPtr) {
    //root.reset(rootPtr, cJSON_Delete);
    //*root = *rootPtr;
}

Json Json::getItem(const char* itemName) const {
    auto item = cJSON_GetObjectItem(extra->self, itemName);
    if(!item)
        throw json_item_not_found{"json field \""s + itemName + "\" not found"s};
    return Json(root, item, extra->self);
}

Json Json::getOrCreateItem(const char* itemName) {
    if(extra->self->type == cJSON_NULL) { //检测是否为Null对象
        extra->self->type = cJSON_Object;
    }
    if(extra->self->type != cJSON_Object) {
        reset(cJSON_Object);
    }
    auto item = cJSON_GetObjectItem(extra->self, itemName);
    if(!item) {
        item = cJSON_CreateNull();
        cJSON_AddItemToObject(extra->self, itemName, item); //必是对象
    }
    return Json(root, item, extra->self); //TODO: 判断是否为null
}

char* Json::copyStr(char* str) {
    auto len = strlen(str);
    auto result = new char[len + 1];
    result[len] = '\0';
    strcpy(result, str);
    return result;
}

namespace std {
string to_string(Json value) {
    auto jsonStr = shared_ptr<char>(cJSON_PrintUnformatted(value.extra->self));
    return jsonStr.get();
}
}

namespace json_literals {
#define DEF_LITERAL(type, li) \
JsonWrapper<type> operator "" _##li (const char *col, size_t n) { \
    return string_view(col, n); \
} \

DEF_LITERAL(int, i);
DEF_LITERAL(std::string, s);
DEF_LITERAL(bool, b);
DEF_LITERAL(nullptr_t, n);
DEF_LITERAL(void, a);
}

