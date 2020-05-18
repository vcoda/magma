namespace magma
{
constexpr DescriptorSetLayout::Binding::Binding(const uint32_t binding, const Descriptor& descriptor, const VkShaderStageFlags stageFlags) noexcept:
    VkDescriptorSetLayoutBinding{
        binding,
        descriptor.type,
        descriptor.descriptorCount,
        stageFlags,
        nullptr // pImmutableSamplers
    }
{}

inline std::size_t DescriptorSetLayout::Binding::hash() const noexcept
{
    return core::hashArgs(
        binding,
        descriptorType,
        descriptorCount,
        stageFlags);
}
} // namespace magma
