// Corresponding header
#include "manager_utils/time/TimerClient.h"
// C system headers
// C++ system headers
#include <iostream>
// Third-party headers
// Own headers
#include "manager_utils/managers/TimerMgr.h"

void TimerClient::startTimer(int64_t interval, int32_t timerId, TimerType timerType){
    if(!gTimerMgr){ // added by Zhivko as a fix in the beginning of Lecture 11 Game 2/4
        return;
    }

    // Listen to this part of the lecture "Buttons_and_Timers" again -> time is around 2:15:00
    constexpr auto minTimerInterval = 20; // milliseconds
    if(interval<minTimerInterval){
        std::cerr<<"Timer with ID: "<<timerId<<" requested interval: "
            <<interval<<" which is lower than the minimum one: "<<minTimerInterval<<std::endl;
        return;
    }

    const TimerData data(interval,interval,this,timerType);
    gTimerMgr->startTimer(data,timerId);
}

void TimerClient::stopTimer(int32_t timerId){
    if(!gTimerMgr){ // added by Zhivko as a fix in the beginning of Lecture 11 Game 2/4
        return;
    }

    gTimerMgr->stopTimer(timerId);
}

bool TimerClient::isActiveTimerId(int32_t timerId) const {
    if(!gTimerMgr){ // added by Zhivko as a fix in the beginning of Lecture 11 Game 2/4
        return false;
    }

    return gTimerMgr->isActiveTimerId(timerId);
}