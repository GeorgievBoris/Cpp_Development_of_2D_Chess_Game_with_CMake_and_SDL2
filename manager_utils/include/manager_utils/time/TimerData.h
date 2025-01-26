#ifndef MANAGER_UTILS_INCLUDE_MANAGER_UTILS_TIME_TIMERDATA_H_
#define MANAGER_UTILS_INCLUDE_MANAGER_UTILS_TIME_TIMERDATA_H_

// C system headers
#include <cstdint>
// C++ system headers
// Third-party headers
// Own headers
// Forward Declarations
class TimerClient;

enum class TimerType:uint8_t{
    ONESHOT,
    PULSE,
    COUNT
};

struct TimerData{
    TimerData(int64_t inputInterval, int64_t inputRemaining, TimerClient* inputTcInstance, TimerType inputTimerType)
            : interval(inputInterval), remaining(inputRemaining), tcInstance(inputTcInstance), timerType(inputTimerType) {}
    int64_t interval;
    int64_t remaining;
    TimerClient* tcInstance=nullptr;
    TimerType timerType;
};

#endif // MANAGER_UTILS_INCLUDE_MANAGER_UTILS_TIME_TIMERDATA_H_