namespace magma
{
constexpr InputAssemblyState::InputAssemblyState(VkPrimitiveTopology topology,
    bool primitiveRestartEnable /* false */):
    VkPipelineInputAssemblyStateCreateInfo{}
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    pNext = nullptr;
    flags = 0;
    this->topology = topology;
    this->primitiveRestartEnable = MAGMA_BOOLEAN(primitiveRestartEnable);
}

constexpr size_t InputAssemblyState::hash() const
{
    return internal::hashArgs(
        sType,
        flags,
        topology,
        primitiveRestartEnable);
}

constexpr bool InputAssemblyState::operator==(const InputAssemblyState& other) const
{
    return (flags == other.flags) &&
        (topology == other.topology) &&
        (primitiveRestartEnable == other.primitiveRestartEnable);
}
} // namespace magma
