#include "cloud.hxx"

using namespace std;


Cloud::Cloud(shared_ptr<CloudTimer> timer): timer(timer) {

}

void Cloud::init() {
    if(timer != nullptr) timer->start();
}
