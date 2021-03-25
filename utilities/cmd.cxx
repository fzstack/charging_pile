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
    try {
        cb(*this);
    } catch(const exception& e) {
        F{} << "error: "_r << (char*)e.what() << endln;
    }
}

void Cmd::assert(bool predict, string msg) {
    if(!predict) {
        throw out_of_range{msg};
    }
}

void Cmd::assert(bool predict, std::function<std::string()> gen) {
    if(!predict) {
        throw out_of_range{gen()};
    }
}

char* Cmd::getFromPos() {
    if(q.empty()) {
        throw out_of_range{"too few args"};
    }
    auto front = q.front();
    q.pop_front();
    return front;
}

char* Cmd::getFromName(const char* name) {
    auto found = m.find(name);
    assert(found != m.end() && found->second != nullptr, "named arg "s + name + " required");
    return found->second;
}

char* Cmd::getFrom(const char* name) {
    if(name == nullptr) {
        return getFromPos();
    } else {
        return getFromName(name);
    }
}
