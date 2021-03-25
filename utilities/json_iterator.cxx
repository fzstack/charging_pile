#include "json_iterator.hxx"

using namespace std;

JsonIterator::JsonIterator(Json json, bool isArr): json(shared_ptr<Json>(new Json(json))), isArr(isArr) {

}

JsonIterator::JsonIterator(): json(nullptr) {

}

JsonIterator& JsonIterator::operator++() {
    if(!json)
        return *this;
    auto self = json->extra->self;
    if(self->next == nullptr) {
        json = nullptr;
    } else {
        json = shared_ptr<Json>(new Json(json->root, self->next, json->extra->parent));
    }
    i++;
    return *this;
}

pair<Json, Json> JsonIterator::operator*() const {
    if(!json)
        throw json_item_not_found{"json iterator out of range"};
    auto self = json->extra->self;
    auto key = Json();

    if(isArr) {
        key = i;
    } else {
        key = self->string;
    }
    return {key, *json};
}

bool JsonIterator::operator!=(const JsonIterator& rhs) {
    if(!json && !rhs.json) {
        return false;
    }
    if((json && rhs.json) && (json->extra->self == rhs.json->extra->self))
        return false;
    return true;
}
