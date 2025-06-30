namespace magma
{
constexpr LineRasterizationState::LineRasterizationState(const RasterizationState& state,
    VkLineRasterizationModeEXT lineRasterizationMode) noexcept:
    RasterizationState(state),
    lineRasterizationInfo{
        VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_LINE_STATE_CREATE_INFO_EXT,
        state.pNext,
        lineRasterizationMode,
        VK_FALSE, // stippledLineEnable
        0, // lineStippleFactor
        0 // lineStipplePattern
    }
{
    pNext = &lineRasterizationInfo;
}

constexpr LineRasterizationState::LineRasterizationState(const RasterizationState& state,
    VkLineRasterizationModeEXT lineRasterizationMode, uint32_t stippleFactor, uint16_t stipplePattern) noexcept:
    RasterizationState(state),
    lineRasterizationInfo{
        VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_LINE_STATE_CREATE_INFO_EXT,
        state.pNext,
        lineRasterizationMode,
        VK_TRUE, // stippledLineEnable
        stippleFactor,
        stipplePattern
    }
{
    pNext = &lineRasterizationInfo;
}

constexpr LineRasterizationState::LineRasterizationState(const LineRasterizationState& other) noexcept:
    RasterizationState(other),
    lineRasterizationInfo(other.lineRasterizationInfo)
{
    pNext = &lineRasterizationInfo;
}

constexpr hash_t LineRasterizationState::hash() const noexcept
{
    hash_t hash = RasterizationState::hash();
    return core::hashCombine(hash, core::hashArgs(
        lineRasterizationInfo.sType,
        lineRasterizationInfo.lineRasterizationMode,
        lineRasterizationInfo.stippledLineEnable,
        lineRasterizationInfo.lineStippleFactor,
        lineRasterizationInfo.lineStipplePattern));
}

constexpr bool LineRasterizationState::operator==(const LineRasterizationState& other) const noexcept
{
    return RasterizationState::operator==(other) &&
        (lineRasterizationInfo.lineRasterizationMode == other.lineRasterizationInfo.lineRasterizationMode) &&
        (lineRasterizationInfo.stippledLineEnable == other.lineRasterizationInfo.stippledLineEnable) &&
        (lineRasterizationInfo.lineStippleFactor == other.lineRasterizationInfo.lineStippleFactor) &&
        (lineRasterizationInfo.lineStipplePattern == other.lineRasterizationInfo.lineStipplePattern);
}
} // namespace magma
