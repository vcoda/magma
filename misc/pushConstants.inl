namespace magma
{
constexpr PushConstantRange::PushConstantRange(const VkShaderStageFlags stageFlags, const uint32_t offset, const std::size_t size) noexcept:
    VkPushConstantRange{
        stageFlags,
        offset,
        static_cast<uint32_t>(size)
    }
{}

inline std::size_t PushConstantRange::hash() const noexcept
{
    return core::hashArgs(
        stageFlags,
        offset,
        size);
}
} // namespace magma
