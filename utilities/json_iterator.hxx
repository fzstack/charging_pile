/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-28     imgcr       the first version
 */
#ifndef UTILITIES_JSON_ITERATOR_HXX_
#define UTILITIES_JSON_ITERATOR_HXX_

#include "json.hxx"
#include "nested.hxx"
#include <tuple>
#include <memory>

class Json;

class JsonIterator {
    friend class Json;
private:
    JsonIterator(Json json, bool isArr);
    JsonIterator();
public:
    JsonIterator& operator++();
    std::pair<Json, Json> operator*() const;
    bool operator!=(const JsonIterator& rhs);

private:
    std::shared_ptr<Json> json;
    int i = 0;
    bool isArr = false;
};

#endif /* UTILITIES_JSON_ITERATOR_HXX_ */
