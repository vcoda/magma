namespace magma
{
constexpr InputAssemblyState::InputAssemblyState(const VkPrimitiveTopology topology,
    const bool primitiveRestartEnable /* false */) noexcept:
    VkPipelineInputAssemblyStateCreateInfo{
        VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        nullptr, // pNext
        0, // flags
        topology,
        MAGMA_BOOLEAN(primitiveRestartEnable)
    }
{}

constexpr std::size_t InputAssemblyState::hash() const noexcept
{
    return core::hashArgs(
        sType,
        flags,
        topology,
        primitiveRestartEnable);
}

constexpr bool InputAssemblyState::operator==(const InputAssemblyState& other) const noexcept
{
    return (flags == other.flags) &&
        (topology == other.topology) &&
        (primitiveRestartEnable == other.primitiveRestartEnable);
}
} // namespace magma
