namespace magma
{
constexpr MinMaxSamplerState::MinMaxSamplerState(const SamplerState& state, const VkSamplerReductionModeEXT reductionMode) noexcept:
    SamplerState(state),
    reductionModeInfo{
        VK_STRUCTURE_TYPE_SAMPLER_REDUCTION_MODE_CREATE_INFO_EXT,
        state.pNext,
        reductionMode
    }
{
    pNext = &reductionModeInfo;
}

constexpr hash_t MinMaxSamplerState::hash() const noexcept
{
    hash_t hash = SamplerState::hash();
    return core::hashCombine(hash, core::hashArgs(
        reductionModeInfo.sType,
        reductionModeInfo.reductionMode));
}

constexpr bool MinMaxSamplerState::operator==(const MinMaxSamplerState& other) const noexcept
{
    return SamplerState::operator==(other) &&
        (reductionModeInfo.reductionMode == other.reductionModeInfo.reductionMode);
}
} // namespace magma
