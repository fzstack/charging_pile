#include "tween.hxx"

using namespace rtthread;

float TweenBase::ease(float x) {
    return x;
}

Mutex TweenBase::mutex = {kMutex};
