/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-03-25     imgcr       the first version
 */

#include "cmd.hxx"
#include <stdexcept>
#include "f.hxx"

using namespace std;
using namespace string_literals;

Cmd::Cmd(int argc, char** argv) {
    auto i = 1;
    while(i < argc) {
        auto currArg = string{argv[i]};
        if(currArg.rfind("--", 0) == 0) { //begin with --
            currArg = currArg.substr(2);
            if(i + 1 < argc && string{argv[i + 1]}.rfind("--", 0) != 0) {
                m[currArg] = argv[i + 1];
                i += 2;
            } else {
                m[currArg] = (char*)"1";
                i++;
            }
        } else {
            q.push_back(argv[i]);
            i++;
        }

    }
}

void Cmd::operator()(std::function<void(Cmd& cmd)> cb) {
#ifdef __cpp_exceptions
    try {
#endif
        cb(*this);
#ifdef __cpp_exceptions
    } catch(const exception& e) {
        F{} << "error: "_r << (char*)e.what() << endln;
    }
#endif
}

void Cmd::assertV(bool predict, string msg) {
    if(!predict) {
#ifdef __cpp_exceptions
        throw out_of_range{msg};
#endif
    }
}

void Cmd::assertV(bool predict, std::function<std::string()> gen) {
    if(!predict) {
#ifdef __cpp_exceptions
        throw out_of_range{gen()};
#endif
    }
}

char* Cmd::getFromPos() {
    if(q.empty()) {
#ifdef __cpp_exceptions
        throw out_of_range{"too few args"};
#endif
    }
    auto front = q.front();
    q.pop_front();
    return front;
}

char* Cmd::getFromName(const char* name) {
    auto found = m.find(name);
    assertV(found != m.end() && found->second != nullptr, "named arg "s + name + " required");
    return found->second;
}

char* Cmd::getFrom(const char* name) {
    if(name == nullptr) {
        return getFromPos();
    } else {
        return getFromName(name);
    }
}
