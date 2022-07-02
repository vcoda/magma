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

constexpr std::size_t MinMaxSamplerState::hash() const noexcept
{
    std::size_t hash = SamplerState::hash();
    return core::hashCombine(hash, core::hashArgs(
        reduction.sType,
        reduction.reductionMode));
}

constexpr bool MinMaxSamplerState::operator==(const MinMaxSamplerState& other) const noexcept
{
    return SamplerState::operator==(other) &&
        (reduction.reductionMode == other.reduction.reductionMode);
}
} // namespace magma
