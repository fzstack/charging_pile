#ifndef COMPONENTS_AIR_HTTP_CLIENT_HXX_
#define COMPONENTS_AIR_HTTP_CLIENT_HXX_

#include "http_client.hxx"
#include <memory>
#include <vector>
#include <components/air_component.hxx>
#include <rtthread.h>

class Air724;
//template<class T> class AirComponent;

class AirHttpClient: public HttpClient, public AirComponent<AirHttpClient> {
    friend class Air724;
private:
    AirHttpClient(std::shared_ptr<Air724> owner);
public:
    virtual std::string send(std::shared_ptr<HttpRequest> request) override;
protected:
    virtual std::vector<at_urc> onUrcTableInit() override;
private:
    struct Events {
        enum Value {
            HttpAction = 1 << 0,
        };
    };

    std::vector<char> recvBuf;
    const std::vector<char>* sendBuf;

    std::shared_ptr<rt_event> event;
};

#endif /* COMPONENTS_AIR_HTTP_CLIENT_HXX_ */
