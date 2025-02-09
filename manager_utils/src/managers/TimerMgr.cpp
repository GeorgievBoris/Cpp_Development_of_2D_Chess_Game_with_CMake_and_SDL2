// Corresponding header
#include "manager_utils/managers/TimerMgr.h"
// C system headers
// C++ system headers
#include <iostream>
// Third-party headers
// Own headers
#include "manager_utils/time/TimerClient.h"

TimerMgr* gTimerMgr=nullptr;

int32_t TimerMgr::init(){
    _elapsedTime.getElapsed();
    return EXIT_SUCCESS;
}

void TimerMgr::deinit(){

}

void TimerMgr::process(){
    removeTimersInternal(); // added here as an improvement - Zhivko mentions it at the beginning of Lecture Game 2/4

    const int64_t msElapsed=_elapsedTime.getElapsed().toMilliseconds();

    for(auto it=_timerMap.begin();it!=_timerMap.end();++it){
        const int32_t timerId=it->first;
        TimerData& timerData=it->second;

        timerData.remaining-=msElapsed;
        if(0>timerData.remaining){
            onTimerTimeout(timerId,timerData);
        }
    }
    removeTimersInternal();
}

void TimerMgr::startTimer(const TimerData& data, int32_t timerId){
    if(isActiveTimerId(timerId)){
        std::cerr<<"Error, trying to start an already existing timer with ID: "<<timerId<<std::endl;
        return;
    }
    _timerMap.emplace(timerId,data);
}

void TimerMgr::stopTimer(int32_t timerId){
    if(isActiveTimerId(timerId)){
        _removeTimerSet.insert(timerId);
        return;
    }
    std::cerr<<"Error, trying to stop a non-existing timer with ID: "<<timerId<<std::endl;
}

bool TimerMgr::isActiveTimerId(int32_t timerId) const{
    return (_removeTimerSet.end()==_removeTimerSet.find(timerId))
            && (_timerMap.end()!=_timerMap.find(timerId));
}

void TimerMgr::onInitEnd(){
    _elapsedTime.getElapsed();
}

size_t TimerMgr::getActiveTimersCount() const{
    return _timerMap.size();
}

void TimerMgr::removeTimersInternal(){
    for(const int32_t timerId :_removeTimerSet){
        auto mapIt=_timerMap.find(timerId);
        if(_timerMap.end()!=mapIt){
            _timerMap.erase(mapIt);
        }
    }
    _removeTimerSet.clear();
}

void TimerMgr::onTimerTimeout(int32_t timerId, TimerData& timer){
    if(TimerType::ONESHOT==timer.timerType){
        _removeTimerSet.insert(timerId);
    }
    timer.tcInstance->onTimeout(timerId);
    timer.remaining+=timer.interval;
}