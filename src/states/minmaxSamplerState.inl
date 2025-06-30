namespace magma
{
constexpr MinmaxSamplerState::MinmaxSamplerState(const SamplerState& state, const VkSamplerReductionModeEXT reductionMode) noexcept:
    SamplerState(state),
    reductionModeInfo{
        VK_STRUCTURE_TYPE_SAMPLER_REDUCTION_MODE_CREATE_INFO_EXT,
        state.pNext,
        reductionMode
    }
{
    pNext = &reductionModeInfo;
}

constexpr MinmaxSamplerState::MinmaxSamplerState(const MinmaxSamplerState& other) noexcept:
    SamplerState(other),
    reductionModeInfo(other.reductionModeInfo)
{
    pNext = &reductionModeInfo;
}

constexpr hash_t MinmaxSamplerState::hash() const noexcept
{
    hash_t hash = SamplerState::hash();
    return core::hashCombine(hash, core::hashArgs(
        reductionModeInfo.sType,
        reductionModeInfo.reductionMode));
}

constexpr bool MinmaxSamplerState::operator==(const MinmaxSamplerState& other) const noexcept
{
    return SamplerState::operator==(other) &&
        (reductionModeInfo.reductionMode == other.reductionModeInfo.reductionMode);
}
} // namespace magma
