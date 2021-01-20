/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-15     imgcr       the first version
 */

#include "http_request.hxx"

using namespace std;

shared_ptr<HttpRequest> HttpRequest::setUrl(string_view url) {
    this->url = url;
    return shared_from_this();
}

shared_ptr<HttpRequest> HttpRequest::addHeader(string_view name, string_view value) {
    this->headers[name.data()] = value;
    return shared_from_this();
}

shared_ptr<HttpRequest> HttpRequest::setBody(vector<char> body) {
    this->body = body;
    return shared_from_this();
}

shared_ptr<HttpRequest> HttpRequest::setBody(string_view body) {
    setBody(body.begin(), body.end());
    return shared_from_this();
}

shared_ptr<HttpRequest> HttpRequest::setMethod(HttpMethod method) {
    this->method = method;
    return shared_from_this();
}

const string_view HttpRequest::getUrl() const {
    return url;
}

const unordered_map<string, string>& HttpRequest::getHeaders() const {
    return headers;
}

const vector<char>& HttpRequest::getBody() const {
    return body;
}

HttpMethod HttpRequest::getMethod() const {
    return method;
}
