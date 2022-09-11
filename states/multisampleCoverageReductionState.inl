namespace magma
{
constexpr MultisampleCoverageReductionState::MultisampleCoverageReductionState(const MultisampleState& state,
    const VkCoverageReductionModeNV coverageReductionMode) noexcept:
    MultisampleState(state),
    coverageReduction{
        VK_STRUCTURE_TYPE_PIPELINE_COVERAGE_REDUCTION_STATE_CREATE_INFO_NV,
        state.pNext,
        0, // flags
        coverageReductionMode
    }
{
    pNext = &coverageReduction;
}

constexpr hash_t MultisampleCoverageReductionState::hash() const noexcept
{
    hash_t hash = MultisampleState::hash();
    return core::hashCombine(hash, core::hashArgs(
        coverageReduction.sType,
        coverageReduction.flags,
        coverageReduction.coverageReductionMode));
}

constexpr bool MultisampleCoverageReductionState::operator==(const MultisampleCoverageReductionState& other) const noexcept
{
    return MultisampleState ::operator==(other) &&
        (coverageReduction.flags == other.coverageReduction.flags) &&
        (coverageReduction.coverageReductionMode == other.coverageReduction.coverageReductionMode);
}
} // namespace magma
