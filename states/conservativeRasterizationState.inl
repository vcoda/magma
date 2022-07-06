constexpr ConservativeRasterizationState::ConservativeRasterizationState(const RasterizationState& state,
    const VkConservativeRasterizationModeEXT conservativeRasterizationMode,
    const float extraPrimitiveOverestimationSize /* 0 */) noexcept:
    RasterizationState(state),
    conservative{
        VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_CONSERVATIVE_STATE_CREATE_INFO_EXT,
        state.pNext,
        0, // flags
        conservativeRasterizationMode,
        extraPrimitiveOverestimationSize
    }
{
    pNext = &conservative;
}

constexpr hash_t ConservativeRasterizationState::hash() const noexcept
{
    hash_t hash = RasterizationState::hash();
    return core::hashCombine(hash, core::hashArgs(
        conservative.sType,
        conservative.flags,
        conservative.conservativeRasterizationMode,
        conservative.extraPrimitiveOverestimationSize));
}

constexpr bool ConservativeRasterizationState::operator==(const ConservativeRasterizationState& other) const noexcept
{
    return RasterizationState::operator==(other) &&
        (conservative.flags == other.conservative.flags) &&
        (conservative.conservativeRasterizationMode == other.conservative.conservativeRasterizationMode) &&
        (conservative.extraPrimitiveOverestimationSize == other.conservative.extraPrimitiveOverestimationSize);
}
