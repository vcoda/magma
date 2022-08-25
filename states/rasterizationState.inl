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

constexpr RasterizationState::RasterizationState(const RasterizationState& other) noexcept:
    VkPipelineRasterizationStateCreateInfo{
        other.sType,
        other.pNext,
        other.flags,
        other.depthClampEnable,
        other.rasterizerDiscardEnable,
        other.polygonMode,
        other.cullMode,
        other.frontFace,
        other.depthBiasEnable,
        other.depthBiasConstantFactor,
        other.depthBiasClamp,
        other.depthBiasSlopeFactor,
        other.lineWidth
    }
{}

constexpr hash_t RasterizationState::hash() const noexcept
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

template<class Node>
inline const Node *RasterizationState::findNode(VkStructureType sType) const noexcept
{
    const VkBaseInStructure *state = reinterpret_cast<const VkBaseInStructure *>(pNext);
    while (state)
    {
        if (state->sType == sType)
            return reinterpret_cast<const Node *>(state);
        state = reinterpret_cast<const VkBaseInStructure *>(state->pNext);
    }
    return nullptr;
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

constexpr DepthBiasRasterizationState::DepthBiasRasterizationState(const RasterizationState& state,
    const float depthBiasConstantFactor, const float depthBiasClamp, const float depthBiasSlopeFactor) noexcept:
    RasterizationState(state)
{
    depthBiasEnable = VK_TRUE;
    this->depthBiasConstantFactor = depthBiasConstantFactor;
    this->depthBiasClamp = depthBiasClamp;
    this->depthBiasSlopeFactor = depthBiasSlopeFactor;
}

constexpr WideLineRasterizationState::WideLineRasterizationState(const RasterizationState& state,
    float lineWidth) noexcept:
    RasterizationState(state)
{
    this->lineWidth = lineWidth;
}

constexpr DiscardRasterizationState::DiscardRasterizationState(const RasterizationState& state) noexcept:
    RasterizationState(state)
{
    rasterizerDiscardEnable = VK_TRUE;
}
} // namespace magma
