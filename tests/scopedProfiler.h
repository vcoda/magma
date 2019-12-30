/*
Magma - C++1x interface over Khronos Vulkan API.
Copyright (C) 2018 Victor Coda.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://www.gnu.org/licenses/>.
*/
#pragma once
#include <chrono>
#include <iostream>
#include <functional>

class ScopedProfiler
{
public:
    ScopedProfiler(const char *message,
        std::function<void(float ms)> callback):
        callback(callback),
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

