namespace magma::core
{
inline void Spinlock::lock() noexcept
{
    while (flag.test_and_set(std::memory_order_acquire))
    {/* The pause intrinsic is used in spin-wait loops
        with the processors implementing dynamic execution
        (especially out-of-order execution). In the spin-
        wait loop, the pause intrinsic improves the speed
        at which the code detects the release of the lock
        and provides especially significant performance gain.
        The execution of the next instruction is delayed
        for an implementation-specific amount of time. */
    #if defined(_M_IX86) || defined(_M_X64) || defined(__i386__) || defined(__x86_64__)
        _mm_pause();
    #elif defined(_M_ARM) || defined(_M_ARM64)
     /* In a Symmetric Multi-Threading (SMT) design,
        a thread can use a Yield instruction to give a hint
        to the processor that it is running on. The Yield
        hint indicates that whatever the thread is currently
        doing is of low importance, and so could yield. For
        example, the thread might be sitting in a spin-lock. */
        __yield();
    #elif defined(__arm__) || defined(__aarch64__)
        #if defined(__ARM_ACLE)
            __yield();
        #elif defined(__has_builtin)
            #if __has_builtin(__builtin_arm_yield)
                __builtin_arm_yield();
            #else
                __asm__ __volatile__("yield");
            #endif
        #else
            __asm__ __volatile__("yield");
        #endif
    #else // !x86-64 && !ARM
        std::this_thread::yield();
    #endif
    }
}

inline void Spinlock::unlock() noexcept
{
    flag.clear(std::memory_order_release);
}
} // namespace magma::core
