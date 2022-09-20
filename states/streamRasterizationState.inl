namespace magma
{
constexpr StreamRasterizationState::StreamRasterizationState(const RasterizationState& state, const uint32_t rasterizationStream,
    const VkPipelineRasterizationStateStreamCreateFlagsEXT flags /* 0 */) noexcept:
    RasterizationState(state),
    streamRasterizationInfo{
        VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_STREAM_CREATE_INFO_EXT,
        state.pNext,
        flags,
        rasterizationStream
    }
{
    pNext = &streamRasterizationInfo;
}

constexpr hash_t StreamRasterizationState::hash() const noexcept
{
    hash_t hash = RasterizationState::hash();
    return core::hashCombine(hash, core::hashArgs(
        streamRasterizationInfo.sType,
        streamRasterizationInfo.flags,
        streamRasterizationInfo.rasterizationStream));
}

constexpr bool StreamRasterizationState::operator==(const StreamRasterizationState& other) const noexcept
{
    return RasterizationState::operator==(other) &&
        (streamRasterizationInfo.flags == other.streamRasterizationInfo.flags) &&
        (streamRasterizationInfo.rasterizationStream == other.streamRasterizationInfo.rasterizationStream);
}
} // namespace magma
