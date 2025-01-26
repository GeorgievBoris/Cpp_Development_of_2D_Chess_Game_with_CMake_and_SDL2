#ifndef MANAGER_UTILS_INCLUDE_MANAGER_UTILS_MANAGERS_TIMERMGR_H_
#define MANAGER_UTILS_INCLUDE_MANAGER_UTILS_MANAGERS_TIMERMGR_H_

// C system headers
#include <cstdint>
// C++ system headers
#include <map>
#include <set>
// Third-party headers
// Own headers
#include "manager_utils/managers/MgrBase.h"
#include "manager_utils/time/TimerData.h"
#include "utils/time/Time.h"
// Forward Declarations

class TimerMgr : public MgrBase {
public:
    int32_t init();
    void deinit() final;
    void process() final;
    void startTimer(const TimerData& data, int32_t timerId);
    void stopTimer(int32_t timerId);
    bool isActiveTimerId(int32_t timerId) const;
    void onInitEnd();

private:
    void removeTimersInternal();
    void onTimerTimeout(int32_t timerId, TimerData& timer);

    Time _elapsedTime;
    std::map<int32_t,TimerData> _timerMap;
    std::set<int32_t> _removeTimerSet;
};

extern TimerMgr* gTimerMgr;


#endif // MANAGER_UTILS_INCLUDE_MANAGER_UTILS_MANAGERS_TIMERMGR_H_