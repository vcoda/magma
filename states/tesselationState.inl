namespace magma
{
constexpr TesselationState::TesselationState(uint32_t patchControlPoints /* 0 */) noexcept
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
    pNext = nullptr;
    flags = 0;
    this->patchControlPoints = patchControlPoints;
}

constexpr size_t TesselationState::hash() const noexcept
{
    return internal::hashArgs(
        flags,
        patchControlPoints);
}

constexpr bool TesselationState::operator==(const TesselationState& other) const noexcept
{
    return (flags == other.flags) && (patchControlPoints == other.patchControlPoints);
}
} // namespace magma
