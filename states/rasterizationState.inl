namespace magma
{
constexpr RasterizationState::RasterizationState(const VkPolygonMode polygonMode,
    const VkCullModeFlags cullMode, const VkFrontFace frontFace,
    const bool depthClampEnable /* false */, const bool rasterizerDiscardEnable /* false */) noexcept:
    VkPipelineRasterizationStateCreateInfo{
        VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        nullptr, // pNext
        0, // flags
        MAGMA_BOOLEAN(depthClampEnable),
        MAGMA_BOOLEAN(rasterizerDiscardEnable),
        polygonMode,
        cullMode,
        frontFace,
        VK_FALSE, // depthBiasEnable
        0.f, // depthBiasConstantFactor
        0.f, // depthBiasClamp
        0.f, // depthBiasSlopeFactor
        1.f  // lineWidth
    }
{}

inline std::size_t RasterizationState::hash() const
{
    return core::hashArgs(
        sType,
        flags,
        depthClampEnable,
        rasterizerDiscardEnable,
        polygonMode,
        cullMode,
        frontFace,
        depthBiasEnable,
        depthBiasConstantFactor,
        depthBiasClamp,
        depthBiasSlopeFactor,
        lineWidth);
}

constexpr bool RasterizationState::operator==(const RasterizationState& other) const
{
    return (flags == other.flags) &&
        (depthClampEnable == other.depthClampEnable) &&
        (rasterizerDiscardEnable == other.rasterizerDiscardEnable) &&
        (polygonMode == other.polygonMode) &&
        (cullMode == other.cullMode) &&
        (frontFace == other.frontFace) &&
        (depthBiasEnable == other.depthBiasEnable) &&
        (depthBiasConstantFactor == other.depthBiasConstantFactor) &&
        (depthBiasClamp == other.depthBiasClamp) &&
        (depthBiasSlopeFactor == other.depthBiasSlopeFactor) &&
        (lineWidth == other.lineWidth);
}

constexpr DepthBiasRasterizationState::DepthBiasRasterizationState(const RasterizationState& state,
    const float depthBiasConstantFactor, const float depthBiasClamp, const float depthBiasSlopeFactor) noexcept:
    RasterizationState(state)
{
    depthBiasEnable = VK_TRUE;
    this->depthBiasConstantFactor = depthBiasConstantFactor;
    this->depthBiasClamp = depthBiasClamp;
    this->depthBiasSlopeFactor = depthBiasSlopeFactor;
}

#ifdef VK_EXT_conservative_rasterization
constexpr ConservativeRasterizationState::ConservativeRasterizationState(const RasterizationState& state,
    const VkConservativeRasterizationModeEXT conservativeRasterizationMode,
    const float extraPrimitiveOverestimationSize /* 0 */) noexcept:
    RasterizationState(state.polygonMode, state.cullMode, state.frontFace, state.depthClampEnable, state.rasterizerDiscardEnable),
    conservative{
        VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_CONSERVATIVE_STATE_CREATE_INFO_EXT,
        nullptr, // pNext
        0, // flags
        conservativeRasterizationMode,
        extraPrimitiveOverestimationSize
    }
{
    pNext = &conservative;
}

inline std::size_t ConservativeRasterizationState::hash() const
{
    std::size_t hash = core::hashArgs(
        conservative.sType,
        conservative.flags,
        conservative.conservativeRasterizationMode,
        conservative.extraPrimitiveOverestimationSize);
    core::hashCombine(hash, RasterizationState::hash());
    return hash;
}

constexpr bool ConservativeRasterizationState::operator==(const ConservativeRasterizationState& other) const
{
    return RasterizationState::operator==(other) &&
        (conservative.flags == other.conservative.flags) &&
        (conservative.conservativeRasterizationMode == other.conservative.conservativeRasterizationMode) &&
        (conservative.extraPrimitiveOverestimationSize == other.conservative.extraPrimitiveOverestimationSize);
}
#endif // VK_EXT_conservative_rasterization

#ifdef VK_AMD_rasterization_order
constexpr RasterizationOrderState::RasterizationOrderState(const RasterizationState& state,
    const VkRasterizationOrderAMD rasterizationOrder) noexcept:
    RasterizationState(state.polygonMode, state.cullMode, state.frontFace, state.depthClampEnable, state.rasterizerDiscardEnable),
    order{
        VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_RASTERIZATION_ORDER_AMD,
        nullptr, // pNext
        rasterizationOrder
    }
{
    pNext = &order;
}

inline std::size_t RasterizationOrderState::hash() const
{
    std::size_t hash = core::hashArgs(
        order.sType,
        order.rasterizationOrder);
    core::hashCombine(hash, RasterizationState::hash());
    return hash;
}

constexpr bool RasterizationOrderState::operator==(const RasterizationOrderState& other) const
{
    return RasterizationState::operator==(other) &&
        (order.rasterizationOrder == other.order.rasterizationOrder);
}
#endif // VK_AMD_rasterization_order
} // namespace magma
