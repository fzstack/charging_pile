#include <components/ota_lower_module.hxx>
#include <vector>
#include <config/co.hxx>

using namespace std;

void OtaLowerModule::start(std::string_view version, std::shared_ptr<IStream> stream, int size) {
    getThread()->exec([this, self = shared_from_this(), stream, size]() mutable {
        rt_kprintf("LOWER OTA RUNNING\n");

        //发送方：create stream
        //接收方：onCreate

        //接收方：read
        //发送方：onRead

        //开始发送数据

        vector<rt_uint8_t> buf(kBufSize);
        int recvedLen, currPos = 0;
        rt_kprintf("erasing parti...\n");
        rpc->invoke<Rpcs::Ota::Erase>({size});
        rt_kprintf("downloading...\n");
        while((recvedLen = stream->readData(&buf[0], kBufSize)) != 0) {
            //TODO: 发送数据给lower单片机
            rpc->invoke<Rpcs::Ota::Write>({currPos, std::move(buf)});
            currPos += recvedLen;
            setProgress(100 * currPos / size);
        }

        //封包：
        //..数据流(upper -> lower): id + 部分数据
        //..数据接收Ack(lower -> upper): id
        //..数据写入完成(lower -> upper): id


        //1.[数据流]
        //2.upper等待lower接收数据，如果x秒后无[数据接收Ack]，则重发数据, 如果x秒内有[数据接收Ack]则等待[数据写入完成]
        //3.lower接收到[数据流]时, 发送每隔y秒发送[数据接收Ack],

    });

}

std::string OtaLowerModule::getName() {
    return "lower";
}

std::string OtaLowerModule::getVersion() {
    return rpc->invoke<Rpcs::Ota::GetVersion>({});;
}
