namespace magma
{
constexpr TesselationState::TesselationState(uint32_t patchControlPoints /* 0 */):
    VkPipelineTessellationStateCreateInfo{}
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
    pNext = nullptr;
    flags = 0;
    this->patchControlPoints = patchControlPoints;
}

constexpr std::size_t TesselationState::hash() const
{
    return core::hashArgs(
        sType,
        flags,
        patchControlPoints);
}

constexpr bool TesselationState::operator==(const TesselationState& other) const
{
    return (flags == other.flags) && (patchControlPoints == other.patchControlPoints);
}
} // namespace magma
