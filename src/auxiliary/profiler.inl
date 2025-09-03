namespace magma::aux
{
inline Profiler::Sample::Sample(const char *name, uint32_t frameIndex, double time) noexcept:
    name(name),
    frameIndex(frameIndex),
    time(time)
{}

inline Profiler::Section::Section(const char *name, uint32_t frameIndex, uint32_t beginQuery) noexcept:
    name(name),
    frameIndex(frameIndex),
    beginQuery(beginQuery)
{}

inline ScopedProfile::ScopedProfile(const char *name, lent_ptr<CommandBuffer> cmdBuffer, uint32_t color /* 0xFFFFFFFF */):
    cmdBuffer(cmdBuffer.get()),
    profiler(Profiler::get(Profiler::Queue::Graphics))
{
    if (profiler)
        profiler->beginSection(std::move(cmdBuffer), name, color);
}

inline ScopedProfile::~ScopedProfile()
{
    if (profiler)
        profiler->endSection(cmdBuffer);
}
} // namespace magma::aux
