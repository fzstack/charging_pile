/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-12-20     imgcr       the first version
 */
#ifndef COMPONENTS_HTTP_FORM_REQUEST_BUILDER_HXX_
#define COMPONENTS_HTTP_FORM_REQUEST_BUILDER_HXX_

#include "http_request.hxx"
#include <string_view>
#include <memory>

class HttpFormRequestBuilder {
public:
    HttpFormRequestBuilder& setUrl(std::string_view url);
    HttpFormRequestBuilder& addParam(std::string_view name, std::string_view value);
    std::shared_ptr<HttpRequest> build();

private:
    std::string url;
    std::map<std::string, std::string> params = {};
};



#endif /* COMPONENTS_HTTP_FORM_REQUEST_BUILDER_HXX_ */
