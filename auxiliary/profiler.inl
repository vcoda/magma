namespace magma
{
namespace aux
{
inline Profiler::Sample::Sample(const char *name, uint32_t frameIndex, double time):
    name(name),
    frameIndex(frameIndex),
    time(time)
{}

inline Profiler::Section::Section(const char *name, uint32_t frameIndex, uint32_t beginQuery):
    name(name),
    frameIndex(frameIndex),
    beginQuery(beginQuery)
{}
} // namespace aux
} // namespace magma
