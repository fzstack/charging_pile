/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-28     imgcr       the first version
 */
#ifndef APPLICATIONS_ALI_IOT_ALINK_HXX_
#define APPLICATIONS_ALI_IOT_ALINK_HXX_

#include <utilities/json.hxx>
#include <string>
#include <string_view>
#include <vector>
#include <optional>

class Alink {
public:
    class Request {
    public:
        Request(Json params, std::string method);
    private:
        Request(Json obj);
    public:
        static Request from(Json req);
        static Request from(std::string& req);
        operator Json();
        operator std::string();

        std::vector<std::string> getMethod();
        Json getParams();
        std::string getId();

    private:
        Json obj;
    };

    class Reply {
    public:
        Reply(Json data = {}, int code = 200, std::optional<std::string> id = std::nullopt);
    private:
        class private_ctor{};
        Reply(private_ctor, Json obj);
    public:
        static Reply from(Json reply);
        static Reply from(std::string& reply);
        operator Json();
        operator std::string();

        int getCode();
        Json getData();

    private:
        Json obj;
    };
private:
    static int id;
    static const char *version;
};



#endif /* APPLICATIONS_ALI_IOT_ALINK_HXX_ */
