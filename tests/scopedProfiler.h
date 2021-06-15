#pragma once
#include <chrono>
#include <iostream>
#include <functional>

class ScopedProfiler
{
public:
    ScopedProfiler(const char *message,
        std::function<void(float ms)> callback):
        callback(std::move(callback)),
        begin(std::chrono::high_resolution_clock::now())
    {
        std::cout << message << std::endl;
    }

    ~ScopedProfiler()
    {
        using namespace std::chrono;
        high_resolution_clock::time_point end = high_resolution_clock::now();
        microseconds mcs = duration_cast<microseconds>(end - begin);
        float ms = static_cast<float>(mcs.count()) / 1000.f;
        callback(ms);
    }

private:
    std::function<void(float ms)> callback;
    std::chrono::high_resolution_clock::time_point begin;
};
