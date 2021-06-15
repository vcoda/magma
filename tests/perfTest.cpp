#ifdef _MSC_VER
#include <malloc.h>
#else
#include <mm_malloc.h>
#endif
#include "scopedProfiler.h"
#include "../core/core.h"
#include "../core/copyMemory.h"
#include "../core/zeroMemory.h"

constexpr size_t BUFFER_SIZE = 1024 * 1024 * 32; // 32M
constexpr int NUM_ITERATIONS = 100;

bool checkBuffer(const void *buffer, char value)
{
    const char *c = (const char *)buffer;
    for (size_t i = 0; i < BUFFER_SIZE; ++i)
    {
        if (c[i] != value)
            return false;
    }
    return true;
}

int main()
{
#if !defined(_M_AMD64) && !defined(__x86_64__)
    std::cout << "Not a x64 build, quit." << std::endl;
    return 0;
#endif
    auto onFinish = [](float ms) -> void
    {
        std::cout << "Elapsed time: " << ms << "ms." << std::endl;
    };
    // Allocate buffers
    void *srcBuffer0 = _mm_malloc(BUFFER_SIZE, 16);
    void *dstBuffer0 = _mm_malloc(BUFFER_SIZE, 16);
    void *srcBuffer1 = _mm_malloc(BUFFER_SIZE, 16);
    void *dstBuffer1 = _mm_malloc(BUFFER_SIZE, 16);
    // Values to be copied
    memset(srcBuffer0, 0x7F, BUFFER_SIZE);
    memset(srcBuffer1, 0x7F, BUFFER_SIZE);
    {   // 1) Test std::memcpy()
        ScopedProfiler prof("Run std::memcpy() performance test.", onFinish);
        for (int i = 0; i < NUM_ITERATIONS; ++i)
            std::memcpy(dstBuffer0, srcBuffer0, BUFFER_SIZE);
    }
    std::cout << std::endl;
    {   // 2) Test our copy function
        ScopedProfiler prof("Run magma::core::copyMemory() performance test.", onFinish);
        for (int i = 0; i < NUM_ITERATIONS; ++i)
            magma::core::copyMemory(dstBuffer1, srcBuffer1, BUFFER_SIZE);
    }
    // Test copy correctness
    if (checkBuffer(dstBuffer1, 0x7F))
        std::cout << "Memory copy Ok" << std::endl;
    else
        std::cout << "Memory copy invalid!" << std::endl;
    std::cout << std::endl;
    {   // 3) Test std::memset()
        ScopedProfiler prof("Run std::memset() performance test.", onFinish);
        for (int i = 0; i < NUM_ITERATIONS; ++i)
            std::memset(dstBuffer0, 0, BUFFER_SIZE);
    }
    std::cout << std::endl;
    {   // 4) Test our zero function
        ScopedProfiler prof("Run magma::core::zeroMemory() performance test.", onFinish);
        for (int i = 0; i < NUM_ITERATIONS; ++i)
            magma::core::zeroMemory(dstBuffer1, BUFFER_SIZE);
    }
    // Test zero correctness
    if (checkBuffer(dstBuffer1, 0))
        std::cout << "Memory zero Ok" << std::endl;
    else
        std::cout << "Memory zero invalid!" << std::endl;
    _mm_free(dstBuffer1);
    _mm_free(srcBuffer1);
    _mm_free(dstBuffer0);
    _mm_free(srcBuffer0);
    return 0;
}
