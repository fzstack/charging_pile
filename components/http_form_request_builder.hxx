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
    std::unordered_map<std::string, std::string> params = {};
};



#endif /* COMPONENTS_HTTP_FORM_REQUEST_BUILDER_HXX_ */
