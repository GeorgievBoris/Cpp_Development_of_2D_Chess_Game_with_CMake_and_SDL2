// Corresponding header
#include "ThreadUtils.h"
// C system headers
// C++ system headers
// Third-party headers
// Own headers
#ifdef _WIND32
#include <windows.h>
#else
#include <thread>
#endif



void ThreadUtils::sleepFor(int64_t microseconds){

    #ifdef _WIN32
        const DWORD milliseconds=std::static_cast<DWORD>(microseconds/1000);
        Sleep(milliseconds); // sleep uses milliseconds
    #else
        // using namespace std::literals;
        // std::this_thread::sleep_for(15ms);

        std::this_thread::sleep_for(std::chrono::microseconds(microseconds));
        // "std::chrono" is an "util" for time in C++

    #endif
}
