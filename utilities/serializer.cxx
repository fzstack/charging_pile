#include "serializer.hxx"

using namespace std;

Serializer::Serializer(shared_ptr<OStream> ostream, function<void(shared_ptr<void>, rt_uint8_t index)> holder): ostream(ostream), holder(holder) {

}

