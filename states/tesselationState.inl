namespace magma
{
constexpr TesselationState::TesselationState(const uint32_t patchControlPoints /* 0 */) noexcept:
    VkPipelineTessellationStateCreateInfo{
        VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO,
        nullptr,
        0, // flags
        patchControlPoints
    }
{}

inline std::size_t TesselationState::hash() const
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
