#ifndef COMPONENTS_HTTP_CLIENT_HXX_
#define COMPONENTS_HTTP_CLIENT_HXX_

#include "http_request.hxx"
#include <string>
#include <memory>

class HttpClient {
public:
    virtual std::string send(std::shared_ptr<HttpRequest> request) = 0;
};

#endif /* COMPONENTS_HTTP_CLIENT_HXX_ */
