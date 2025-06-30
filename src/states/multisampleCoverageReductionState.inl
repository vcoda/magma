namespace magma
{
constexpr MultisampleCoverageReductionState::MultisampleCoverageReductionState(const MultisampleState& state,
    const VkCoverageReductionModeNV coverageReductionMode) noexcept:
    MultisampleState(state),
    coverageReductionInfo{
        VK_STRUCTURE_TYPE_PIPELINE_COVERAGE_REDUCTION_STATE_CREATE_INFO_NV,
        state.pNext,
        0, // flags
        coverageReductionMode
    }
{
    pNext = &coverageReductionInfo;
}

constexpr MultisampleCoverageReductionState::MultisampleCoverageReductionState(const MultisampleCoverageReductionState& other) noexcept:
    MultisampleState(other),
    coverageReductionInfo(other.coverageReductionInfo)
{
    pNext = &coverageReductionInfo;
}

constexpr hash_t MultisampleCoverageReductionState::hash() const noexcept
{
    hash_t hash = MultisampleState::hash();
    return core::hashCombine(hash, core::hashArgs(
        coverageReductionInfo.sType,
        coverageReductionInfo.flags,
        coverageReductionInfo.coverageReductionMode));
}

constexpr bool MultisampleCoverageReductionState::operator==(const MultisampleCoverageReductionState& other) const noexcept
{
    return MultisampleState ::operator==(other) &&
        (coverageReductionInfo.flags == other.coverageReductionInfo.flags) &&
        (coverageReductionInfo.coverageReductionMode == other.coverageReductionInfo.coverageReductionMode);
}
} // namespace magma
