namespace magma
{
constexpr LineRasterizationState::LineRasterizationState(const RasterizationState& state,
    VkLineRasterizationModeEXT lineRasterizationMode) noexcept:
    RasterizationState(state),
    line{
        VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_LINE_STATE_CREATE_INFO_EXT,
        state.pNext,
        lineRasterizationMode,
        VK_FALSE, // stippledLineEnable
        0, // lineStippleFactor
        0 // lineStipplePattern
    }
{
    pNext = &line;
}

constexpr LineRasterizationState::LineRasterizationState(const RasterizationState& state,
    VkLineRasterizationModeEXT lineRasterizationMode, uint32_t stippleFactor, uint16_t stipplePattern) noexcept:
    RasterizationState(state),
    line{
        VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_LINE_STATE_CREATE_INFO_EXT,
        state.pNext,
        lineRasterizationMode,
        VK_TRUE, // stippledLineEnable
        stippleFactor,
        stipplePattern
    }
{
    pNext = &line;
}

constexpr hash_t LineRasterizationState::hash() const noexcept
{
    hash_t hash = RasterizationState::hash();
    return core::hashCombine(hash, core::hashArgs(
        line.sType,
        line.lineRasterizationMode,
        line.stippledLineEnable,
        line.lineStippleFactor,
        line.lineStipplePattern));
}

constexpr bool LineRasterizationState::operator==(const LineRasterizationState& other) const noexcept
{
    return RasterizationState::operator==(other) &&
        (line.lineRasterizationMode == other.line.lineRasterizationMode) &&
        (line.stippledLineEnable == other.line.stippledLineEnable) &&
        (line.lineStippleFactor == other.line.lineStippleFactor) &&
        (line.lineStipplePattern == other.line.lineStipplePattern);
}
} // namespace magma
