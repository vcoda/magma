namespace magma
{
constexpr RasterizationState::RasterizationState(VkPolygonMode polygonMode, VkCullModeFlags cullMode, VkFrontFace frontFace,
    bool depthClampEnable /* false */, bool rasterizerDiscardEnable /* false */):
    VkPipelineRasterizationStateCreateInfo{}
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    pNext = nullptr;
    flags = 0;
    this->depthClampEnable = MAGMA_BOOLEAN(depthClampEnable);
    this->rasterizerDiscardEnable = MAGMA_BOOLEAN(rasterizerDiscardEnable);
    this->polygonMode = polygonMode;
    this->cullMode = cullMode;
    this->frontFace = frontFace;
    depthBiasEnable = VK_FALSE;
    depthBiasConstantFactor = 0.f;
    depthBiasClamp = 0.f;
    depthBiasSlopeFactor = 0.f;
    lineWidth = 1.f;
}

constexpr std::size_t RasterizationState::hash() const
{
    return detail::hashArgs(
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

constexpr ConservativeRasterizationState::ConservativeRasterizationState(const RasterizationState& state,
    VkConservativeRasterizationModeEXT conservativeRasterizationMode,
    float extraPrimitiveOverestimationSize /* 0 */):
    RasterizationState(state.polygonMode, state.cullMode, state.frontFace, state.depthClampEnable, state.rasterizerDiscardEnable),
    conservative{}
{
    conservative.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_CONSERVATIVE_STATE_CREATE_INFO_EXT;
    conservative.pNext = nullptr;
    conservative.flags = 0;
    conservative.conservativeRasterizationMode = conservativeRasterizationMode;
    conservative.extraPrimitiveOverestimationSize = extraPrimitiveOverestimationSize;
    pNext = &conservative;
}

constexpr std::size_t ConservativeRasterizationState::hash() const
{
    return detail::hashArgs(
        RasterizationState::hash(),
        conservative.sType,
        conservative.flags,
        conservative.conservativeRasterizationMode,
        conservative.extraPrimitiveOverestimationSize);
}

constexpr bool ConservativeRasterizationState::operator==(const ConservativeRasterizationState& other) const
{
    return RasterizationState::operator==(other) &&
        (conservative.flags == other.conservative.flags) &&
        (conservative.conservativeRasterizationMode == other.conservative.conservativeRasterizationMode) &&
        (conservative.extraPrimitiveOverestimationSize == other.conservative.extraPrimitiveOverestimationSize);
}

constexpr DepthBiasRasterizationState::DepthBiasRasterizationState(const RasterizationState& state,
    float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor):
    RasterizationState(state.polygonMode, state.cullMode, state.frontFace, state.depthClampEnable, state.rasterizerDiscardEnable)
{
    this->depthBiasEnable = VK_TRUE;
    this->depthBiasConstantFactor = depthBiasConstantFactor;
    this->depthBiasClamp = depthBiasClamp;
    this->depthBiasSlopeFactor = depthBiasSlopeFactor;
}

constexpr RasterizationOrderState::RasterizationOrderState(const RasterizationState& state,
    VkRasterizationOrderAMD rasterizationOrder):
    RasterizationState(state.polygonMode, state.cullMode, state.frontFace, state.depthClampEnable, state.rasterizerDiscardEnable),
    order{}
{
    order.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_RASTERIZATION_ORDER_AMD;
    order.pNext = nullptr;
    order.rasterizationOrder = rasterizationOrder;
    pNext = &order;
}

constexpr std::size_t RasterizationOrderState::hash() const
{
    return detail::hashArgs(
        RasterizationState::hash(),
        order.sType,
        order.rasterizationOrder);
}

constexpr bool RasterizationOrderState::operator==(const RasterizationOrderState& other) const
{
    return RasterizationState::operator==(other) &&
        (order.rasterizationOrder == other.order.rasterizationOrder);
}
} // namespace magma
