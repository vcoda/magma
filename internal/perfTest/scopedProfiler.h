#include <chrono>
#include <iostream>

class ScopedProfiler
{
    typedef std::chrono::high_resolution_clock HiResClock;

public:
    ScopedProfiler(const char *message):
        begin(HiResClock::now())
    {
        std::cout << message << std::endl;
    }
    ~ScopedProfiler()
    {
        const HiResClock::time_point end = HiResClock::now();
        const std::chrono::microseconds mcs = 
            std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
        const float ms = static_cast<float>(mcs.count()) / 1000.0f;
        std::cout << "Elapsed time: " << ms << "ms." << std::endl;
    }

private:
    HiResClock::time_point begin;
};