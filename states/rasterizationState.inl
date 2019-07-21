namespace magma
{
constexpr RasterizationState::RasterizationState(VkPolygonMode polygonMode, VkCullModeFlags cullMode, VkFrontFace frontFace,
    bool depthClampEnable /* false */, bool rasterizerDiscardEnable /* false */) noexcept
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

constexpr size_t RasterizationState::hash() const noexcept
{
    return internal::hashArgs(
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

constexpr bool RasterizationState::operator==(const RasterizationState& other) const noexcept
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
    float extraPrimitiveOverestimationSize /* 0 */) noexcept:
    RasterizationState(state.polygonMode, state.cullMode, state.frontFace, state.depthClampEnable, state.rasterizerDiscardEnable)
{
    conservative.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_CONSERVATIVE_STATE_CREATE_INFO_EXT;
    conservative.pNext = nullptr;
    conservative.flags = 0;
    conservative.conservativeRasterizationMode = conservativeRasterizationMode;
    conservative.extraPrimitiveOverestimationSize = extraPrimitiveOverestimationSize;
    pNext = &conservative;
}

constexpr size_t ConservativeRasterizationState::hash() const noexcept
{
    return internal::hashArgs(
        RasterizationState::hash(),
        conservative.flags,
        conservative.conservativeRasterizationMode,
        conservative.extraPrimitiveOverestimationSize);
}

constexpr bool ConservativeRasterizationState::operator==(const ConservativeRasterizationState& other) const noexcept
{
    return RasterizationState::operator==(other) &&
        (conservative.flags == other.conservative.flags) &&
        (conservative.conservativeRasterizationMode == other.conservative.conservativeRasterizationMode) &&
        (conservative.extraPrimitiveOverestimationSize == other.conservative.extraPrimitiveOverestimationSize);
}

constexpr DepthBiasRasterizationState::DepthBiasRasterizationState(const RasterizationState& state,
    float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor) noexcept:
    RasterizationState(state.polygonMode, state.cullMode, state.frontFace, state.depthClampEnable, state.rasterizerDiscardEnable)
{
    this->depthBiasEnable = VK_TRUE;
    this->depthBiasConstantFactor = depthBiasConstantFactor;
    this->depthBiasClamp = depthBiasClamp;
    this->depthBiasSlopeFactor = depthBiasSlopeFactor;
}

constexpr RasterizationOrderState::RasterizationOrderState(const RasterizationState& state,
    VkRasterizationOrderAMD rasterizationOrder) noexcept:
    RasterizationState(state.polygonMode, state.cullMode, state.frontFace, state.depthClampEnable, state.rasterizerDiscardEnable)
{
    order.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_RASTERIZATION_ORDER_AMD;
    order.pNext = nullptr;
    order.rasterizationOrder = rasterizationOrder;
    pNext = &order;
}

constexpr size_t RasterizationOrderState::hash() const noexcept
{
    return internal::hashArgs(
        RasterizationState::hash(),
        order.rasterizationOrder);
}

constexpr bool RasterizationOrderState::operator==(const RasterizationOrderState& other) const noexcept
{
    return RasterizationState::operator==(other) &&
        (order.rasterizationOrder == other.order.rasterizationOrder);
}
} // namespace magma
