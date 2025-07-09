namespace magma
{
constexpr ProvokingVertexRasterizationState::ProvokingVertexRasterizationState(const RasterizationState& state,
    const VkProvokingVertexModeEXT provokingVertexMode) noexcept:
    RasterizationState(state),
    provokingVertexInfo{
        VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_PROVOKING_VERTEX_STATE_CREATE_INFO_EXT,
        state.pNext,
        provokingVertexMode
    }
{
    pNext = &provokingVertexInfo;
}

constexpr ProvokingVertexRasterizationState::ProvokingVertexRasterizationState(const ProvokingVertexRasterizationState& other) noexcept:
    RasterizationState(other),
    provokingVertexInfo(other.provokingVertexInfo)
{
    pNext = &provokingVertexInfo;
}

constexpr hash_t ProvokingVertexRasterizationState::hash() const noexcept
{
    hash_t hash = RasterizationState::hash();
    return core::hashCombine(hash, core::hashArgs(
        provokingVertexInfo.sType,
        provokingVertexInfo.provokingVertexMode));
}

constexpr bool ProvokingVertexRasterizationState::operator==(const ProvokingVertexRasterizationState& other) const noexcept
{
    return RasterizationState::operator==(other) &&
        (provokingVertexInfo.provokingVertexMode == other.provokingVertexInfo.provokingVertexMode);
}
} // namespace magma
