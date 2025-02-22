#ifndef MANAGER_UTILS_INCLUDE_MANAGER_UTILS_TIME_TIMERCLIENT_H_
#define MANAGER_UTILS_INCLUDE_MANAGER_UTILS_TIME_TIMERCLIENT_H_

// C system headers
#include <cstdint>
// C++ system headers
#include <vector>
// Third-party headers
// Own headers
#include "manager_utils/time/TimerData.h"
// Forward Declarations

class TimerClient{
public:
    // advice from Zhivko to use the dtor of the base class in order to stop all timers
    // this is a smart way of stopping all active timers...
    // ... compared to using the dtor of the class that inherits TimerClient
    virtual ~TimerClient()=default;
    virtual void onTimeout(int32_t timerId)=0;
    void startTimer(int64_t interval, int32_t timerId, TimerType timerType);
    void stopTimer(int32_t timerId);
    bool isActiveTimerId(int32_t timerId) const;
};


#endif // MANAGER_UTILS_INCLUDE_MANAGER_UTILS_TIME_TIMERCLIENT_H_