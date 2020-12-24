/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-20     imgcr       the first version
 */

#include "http_form_request_builder.hxx"
#include <string>

using namespace std;
using namespace std::string_literals;

HttpFormRequestBuilder& HttpFormRequestBuilder::setUrl(std::string_view url) {
    this->url = url;
    return *this;
}

HttpFormRequestBuilder& HttpFormRequestBuilder::addParam(std::string_view name, std::string_view value) {
    params[name.data()] = value;
    return *this;
}

shared_ptr<HttpRequest> HttpFormRequestBuilder::build() {
    auto body = ""s;
    for(const auto& kvp: params) {
        body += kvp.first.data() + "="s + kvp.second.data() + "&"s;
    }
    body.pop_back();

    return make_shared<HttpRequest>()
        ->setUrl(url)
        ->setMethod(HttpMethod::Post)
        ->addHeader("Content-Type", "application/x-www-form-urlencoded")
        ->setBody(body);
}

