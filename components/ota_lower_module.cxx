#include <components/ota_lower_module.hxx>
#include <config/co.hxx>

using namespace std;

void OtaLowerModule::start(std::string_view version, std::shared_ptr<IStream> stream, int size) {
    currPos = 0;
    recvedLen = 0;

    cb = [this, self = shared_from_this(), stream, size](variant<Void, exception_ptr> rv){
        getThread()->exec([this, self, stream, size, rv]{ //low线程
            auto err = get_if<exception_ptr>(&rv);
            if(err) {
                //重新写入数据
                rt_uint32_t memUsed;
                rt_memory_info(nullptr, &memUsed, nullptr);
                rpc->invoke<Rpcs::Ota::Write>({currPos, buf}, cb, 1000); //low线程返回
            } else {
                //读取下一个数据
                currPos += recvedLen;
                setProgress(100 * currPos / size);
                try {
                    buf.resize(kBufSize);
                    recvedLen = stream->readData(&buf[0], kBufSize);
                    buf.resize(recvedLen);
                } catch(exception& e) {
                    emitError(OtaError::DownloadFailed, "下载失败");
                    return;
                }
                if(recvedLen == 0) {
                    if(currPos == size) {
                        emitDone();
                    } else {
                        emitError(OtaError::DownloadFailed, "下载失败");
                    }
                    return;
                }
                cb(make_exception_ptr(not_implemented{"write"})); //low线程
            }
        });
    };

    getThread()->exec([this, self = shared_from_this(), stream, size]() mutable {
        rt_kprintf("LOWER OTA RUNNING\n");
        rt_kprintf("erasing parti...\n");
        rpc->invoke<Rpcs::Ota::Erase>({size}, [this, self, stream, size](auto r) {
            getThread()->exec([=]{
                auto err = get_if<exception_ptr>(&r);
                if(err != nullptr) {
                    emitError(OtaError::UpdateFailed, "分区擦除失败");
                    return;
                }
                rt_kprintf("downloading...\n");
                cb(Void{});
            });
        });
    });
}

std::string OtaLowerModule::getName() {
    return "lower";
}

void OtaLowerModule::getVersion(std::function<void(std::variant<std::string, std::exception_ptr>)> cb) {
    rpc->invoke<Rpcs::Ota::GetVersion>({}, cb);
}

