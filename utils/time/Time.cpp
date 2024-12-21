// Corresponding header
#include "Time.h"
// C system headers
// C++ system headers
// Third-party headers
// Own headers

Time& Time::getElapsed(){
    _elapsedLastCall=_now;
    _now=std::chrono::steady_clock::now();

    return *this;

}

int64_t Time::toSeconds() const{
    return std::chrono::duration_cast<std::chrono::seconds>(_now-_elapsedLastCall).count();
}

int64_t Time::toMilliseconds() const{
    return std::chrono::duration_cast<std::chrono::milliseconds>(_now-_elapsedLastCall).count();
}

int64_t Time::toMicroseconds() const{
    return std::chrono::duration_cast<std::chrono::microseconds>(_now-_elapsedLastCall).count();
}

int64_t Time::toNanoseconds() const{
    return std::chrono::duration_cast<std::chrono::nanoseconds>(_now-_elapsedLastCall).count();
}