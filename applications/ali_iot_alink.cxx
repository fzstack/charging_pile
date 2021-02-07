/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-28     imgcr       the first version
 */
#include <string>
#include <utilities/string.hxx>
#include "ali_iot_alink.hxx"

using namespace std;
using namespace json_literals;

Alink::Request::Request(Json params, std::string method): obj({
    {"id", to_string(Alink::id)},
    {"version", Alink::version},
    {"params", params},
    {"method", method},
}) {
    Alink::id++;
}

Alink::Request::Request(Json obj): obj(obj) {

}

Alink::Request::operator Json() {
    return obj;
}

Alink::Request::operator std::string() {
    return to_string(obj);
}

Alink::Request Alink::Request::from(Json req) {
    return Request(req);
}

Alink::Request Alink::Request::from(string& req) {
    return Request(Json::parse(req));
}

std::vector<std::string> Alink::Request::getMethod() {
    return split(obj["method"_s], '.');
}

Json Alink::Request::getParams() {
    return obj["params"];
}

std::string Alink::Request::getId() {
    return obj["id"];
}

Alink::Reply::Reply(Json data, int code, std::optional<std::string> id): obj({
    {"code", code},
    {"data", data},
}) {
    if(id == nullopt) {
        obj["id"] = Json(to_string(Alink::id));
        Alink::id++;
    } else {
        obj["id"] = Json(*id);
    }
}

Alink::Reply::Reply(private_ctor, Json obj): obj(obj) {

}

Alink::Reply Alink::Reply::from(Json reply) {
    return Reply(private_ctor{}, reply);
}

Alink::Reply Alink::Reply::from(string& reply) {
    return Reply(private_ctor{}, Json::parse(reply));
}

Alink::Reply::operator Json() {
    return obj;
}

Alink::Reply::operator std::string() {
    return to_string(obj);
}

int Alink::Reply::getCode() {
    return obj["code"_i];
}

Json Alink::Reply::getData() {
    return obj["data"];
}

int Alink::id = 0;
const char *Alink::version = "1.0";

