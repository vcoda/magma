namespace magma
{
constexpr ConservativeRasterizationState::ConservativeRasterizationState(const RasterizationState& state,
    const VkConservativeRasterizationModeEXT conservativeRasterizationMode,
    const float extraPrimitiveOverestimationSize /* 0 */) noexcept:
    RasterizationState(state),
    conservativeRasterizationInfo{
        VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_CONSERVATIVE_STATE_CREATE_INFO_EXT,
        state.pNext,
        0, // flags
        conservativeRasterizationMode,
        extraPrimitiveOverestimationSize
    }
{
    pNext = &conservativeRasterizationInfo;
}

constexpr hash_t ConservativeRasterizationState::hash() const noexcept
{
    hash_t hash = RasterizationState::hash();
    return core::hashCombine(hash, core::hashArgs(
        conservativeRasterizationInfo.sType,
        conservativeRasterizationInfo.flags,
        conservativeRasterizationInfo.conservativeRasterizationMode,
        conservativeRasterizationInfo.extraPrimitiveOverestimationSize));
}

constexpr bool ConservativeRasterizationState::operator==(const ConservativeRasterizationState& other) const noexcept
{
    return RasterizationState::operator==(other) &&
        (conservativeRasterizationInfo.flags == other.conservativeRasterizationInfo.flags) &&
        (conservativeRasterizationInfo.conservativeRasterizationMode == other.conservativeRasterizationInfo.conservativeRasterizationMode) &&
        (conservativeRasterizationInfo.extraPrimitiveOverestimationSize == other.conservativeRasterizationInfo.extraPrimitiveOverestimationSize);
}
} // namespace magma
