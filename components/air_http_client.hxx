#ifndef COMPONENTS_AIR_HTTP_CLIENT_HXX_
#define COMPONENTS_AIR_HTTP_CLIENT_HXX_

#include "http_client.hxx"
#include <memory>
#include <vector>
#include <components/air_component.hxx>
#include <rtthread.h>
#include <utilities/nested.hxx>

class Air724;
//template<class T> class AirComponent;

class AirHttpClient: public HttpClient, public AirComponent<AirHttpClient> {
    friend class Air724;
    using p_t = AirComponent<AirHttpClient>;
private:
    AirHttpClient(std::shared_ptr<Air724> owner);
public:
    virtual std::string send(std::shared_ptr<HttpRequest> request) override;

private:
    struct Session {
        std::shared_ptr<at_response> resp;
        std::shared_ptr<void> termGuard;
    };

    class Stream: public IStream, public Nested<AirHttpClient> {
    public:
        Stream(outer_t* outer, Session sess);
        virtual int readData(rt_uint8_t* data, int len) override final;
    private:
        int currPos = 0;
        Session sess;
    };
    friend class Stream;

public:
    virtual std::shared_ptr<IStream> stream(std::shared_ptr<HttpRequest> request) override;
protected:
    virtual std::vector<at_urc> onUrcTableInit() override;
private:
    Session sendInternal(std::shared_ptr<HttpRequest> request, int timeout = p_t::kTimeout);
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
