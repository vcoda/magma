namespace magma
{
constexpr InputAssemblyState::InputAssemblyState(VkPrimitiveTopology topology,
    bool primitiveRestartEnable /* false */) noexcept
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    pNext = nullptr;
    flags = 0;
    this->topology = topology;
    this->primitiveRestartEnable = MAGMA_BOOLEAN(primitiveRestartEnable);
}

constexpr size_t InputAssemblyState::hash() const noexcept
{
    return internal::hashArgs(
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
