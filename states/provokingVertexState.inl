namespace magma
{
constexpr ProvokingVertexRasterizationState::ProvokingVertexRasterizationState(const RasterizationState& state,
    const VkProvokingVertexModeEXT provokingVertexMode) noexcept:
    RasterizationState(state),
    provoking{
        VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_PROVOKING_VERTEX_STATE_CREATE_INFO_EXT,
        state.pNext,
        provokingVertexMode
    }
{
    pNext = &provoking;
}

constexpr hash_t ProvokingVertexRasterizationState::hash() const noexcept
{
    hash_t hash = RasterizationState::hash();
    return core::hashCombine(hash, core::hashArgs(
        provoking.sType,
        provoking.provokingVertexMode));
}

constexpr bool ProvokingVertexRasterizationState::operator==(const ProvokingVertexRasterizationState& other) const noexcept
{
    return RasterizationState::operator==(other) &&
        (provoking.provokingVertexMode == other.provoking.provokingVertexMode);
}
} // namespace magma
