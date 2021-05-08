/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-04-19     imgcr       the first version
 */
#pragma once
#include <rtthread.h>
#include <memory>
#include <functional>
#include <variant>
#include <stdexcept>

#ifndef TEST_RETRY
#define rt_kprintf(x)
#endif

template<class T>
class Retry: public std::enable_shared_from_this<Retry<T>> {
    using cb_fetch_t = std::function<void(std::variant<T, std::exception_ptr>)>;
    using cb_run_t = std::function<void(cb_fetch_t)>;
    using cb_r_t = std::function<void(T)>;
    using cb_e_t = std::function<void()>;
private:
    class Starter {
        friend class Retry<T>;
    private:
        Starter(std::shared_ptr<Retry<T>> retry, cb_run_t cb): retry(retry), cbRun(cb) {};
    public:
        ~Starter() {
            rt_kprintf("running\n");
            retry->run(cbRun);
        }

        Starter& then(cb_r_t cb) {
            rt_kprintf("set cb r\n");
            retry->setCbR(cb);
            return *this;
        }

        Starter& except(cb_e_t cb) {

            rt_kprintf("set cb e\n");
            retry->setCbE(cb);
            return *this;
        }

    private:
        std::shared_ptr<Retry<T>> retry;
        cb_run_t cbRun;
    };
    friend class Starter;

public:
    template<class ...Args>
    static auto start(cb_run_t cb, Args ... args) {
        auto retry = std::shared_ptr<Retry<T>>(new Retry<T>(std::forward<Args>(args)...));
        return Starter{retry, cb};
    }

    ~Retry() {
        rt_kprintf("retry dector\n");
    }

private:
    Retry(int maxCnt = kMaxCnt): cnt(maxCnt) {

    }

    //run 函数内不能持有retry智能指针
    void run(cb_run_t cb) {
        cbRun = cb;
        cb(fetch());
    }

    void setCbR(cb_r_t cb) {
        cbR = cb;
    }

    void setCbE(cb_e_t cb) {
        cbE = cb;
    }

    //DO NOT CALL THIS FUNC ON CTOR
    auto fetch() -> cb_fetch_t {
        return [this, self = this->shared_from_this()](auto r){
            auto pres = std::get_if<T>(&r);
            if(pres == nullptr) {
                std::get<std::exception_ptr>(r) = nullptr;
                if(cnt > 0) {
                    --cnt;
                    cbRun(fetch());
                } else {
                    if(cbE) {
                        cbE();
                    } else {
                        rt_kprintf("W: too many retry attempts");
                    }
                }
            } else {
                if(cbR) {
                    cbR(std::move(*pres));
                }
            }
        };
    }

private:
    int cnt;
    cb_run_t cbRun = {};
    cb_r_t cbR = {};
    cb_e_t cbE = {};

public:
    static constexpr int kMaxCnt = 5;
};

#ifndef TEST_RETRY
#undef rt_kprintf
#endif
