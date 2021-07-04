#ifndef COMPONENTS_HTTP_REQUEST_HXX_
#define COMPONENTS_HTTP_REQUEST_HXX_

#include <string_view>
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <unordered_map>

enum class HttpMethod {
    Get,
    Post,
};

class HttpRequest: public std::enable_shared_from_this<HttpRequest> {
public:
    std::shared_ptr<HttpRequest> setUrl(std::string_view url);
    std::shared_ptr<HttpRequest> addHeader(std::string_view name, std::string_view value);
    std::shared_ptr<HttpRequest> setBody(std::vector<char> body);
    std::shared_ptr<HttpRequest> setBody(std::string_view body);
    template<class InputIt>
    std::shared_ptr<HttpRequest> setBody(InputIt first, InputIt last) {
        body = move(std::vector<char>(first, last));
        return shared_from_this();
    }
    std::shared_ptr<HttpRequest> setMethod(HttpMethod method);

    const std::string_view getUrl() const;
    const std::unordered_map<std::string, std::string>& getHeaders() const;
    const std::vector<char>& getBody() const;
    HttpMethod getMethod() const;

private:
    std::string url;
    std::unordered_map<std::string, std::string> headers = {};
    std::vector<char> body;
    HttpMethod method;
};



#endif /* COMPONENTS_HTTP_CLIENT_HXX_ */
