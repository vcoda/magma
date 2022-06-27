namespace magma
{
constexpr MinMaxSamplerState::MinMaxSamplerState(const SamplerState& state, const VkSamplerReductionModeEXT reductionMode) noexcept:
    SamplerState(state),
    reduction{
        VK_STRUCTURE_TYPE_SAMPLER_REDUCTION_MODE_CREATE_INFO_EXT,
        state.pNext,
        reductionMode
    }
{
    pNext = &reduction;
}

inline std::size_t MinMaxSamplerState::hash() const noexcept
{
    std::size_t hash = SamplerState::hash();
    core::hashCombine(hash, core::hashArgs(
        reduction.sType,
        reduction.reductionMode));
    return hash;
}

constexpr bool MinMaxSamplerState::operator==(const MinMaxSamplerState& other) const noexcept
{
    return SamplerState::operator==(other) &&
        (reduction.reductionMode == other.reduction.reductionMode);
}
} // namespace magma
