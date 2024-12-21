#ifndef UTILS_THREAD_THREADUTILS_H_
#define UTILS_THREAD_THREADUTILS_H_

// C system headers
#include <cstdint>
// C++ system headers
// Third-party headers
// Own headers
// Forward Declarations



class ThreadUtils {

public:

    ThreadUtils()=delete;
    ~ThreadUtils()=default;

    static void sleepFor(int64_t microseconds);

private:


};


#endif // UTILS_THREAD_THREADUTILS_H_