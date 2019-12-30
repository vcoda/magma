namespace magma
{
constexpr std::size_t StencilOpState::hash() const
{
    return detail::hashArgs(
        failOp,
        passOp,
        depthFailOp,
        compareOp,
        compareMask,
        writeMask,
        reference);
}

constexpr bool StencilOpState::operator==(const StencilOpState& other) const
{
    return (failOp == other.failOp) &&
        (passOp == other.passOp) &&
        (depthFailOp == other.depthFailOp) &&
        (compareOp == other.compareOp) &&
        (compareMask == other.compareMask) &&
        (writeMask == other.writeMask) &&
        (reference == other.reference);
}

constexpr DepthStencilState::DepthStencilState(VkCompareOp depthCompareOp, bool depthWriteEnable,
    const StencilOpState& front, const StencilOpState& back):
    VkPipelineDepthStencilStateCreateInfo{}
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    pNext = nullptr;
    flags = 0;
    depthTestEnable = (depthCompareOp != VK_COMPARE_OP_ALWAYS);
    this->depthWriteEnable = MAGMA_BOOLEAN(depthWriteEnable);
    this->depthCompareOp = depthCompareOp;
    this->front = front;
    this->back = back;
    stencilTestEnable = (front.compareOp != VK_COMPARE_OP_ALWAYS) || (back.compareOp != VK_COMPARE_OP_ALWAYS);
    depthBoundsTestEnable = VK_FALSE;
    minDepthBounds = 0.f;
    maxDepthBounds = 1.f;
}

constexpr DepthStencilState::DepthStencilState(const DepthStencilState& state,
    const StencilOpState& front, const StencilOpState& back):
    DepthStencilState(state.depthCompareOp, state.depthCompareOp, front, back)
{}

constexpr std::size_t DepthStencilState::hash() const
{
    return detail::hashArgs(
        sType,
        flags,
        depthTestEnable,
        depthWriteEnable,
        depthCompareOp,
        front.failOp,
        front.passOp,
        front.depthFailOp,
        front.compareOp,
        front.compareMask,
        front.writeMask,
        front.reference,
        back.failOp,
        back.passOp,
        back.depthFailOp,
        back.compareOp,
        back.compareMask,
        back.writeMask,
        back.reference,
        stencilTestEnable,
        depthBoundsTestEnable,
        minDepthBounds,
        maxDepthBounds);
}

constexpr bool DepthStencilState::operator==(const DepthStencilState& other) const
{
    return (flags == other.flags) &&
        (depthTestEnable == other.depthTestEnable) &&
        (depthWriteEnable == other.depthWriteEnable) &&
        (depthCompareOp == other.depthCompareOp) &&
        (detail::compare(&front, &other.front)) &&
        (detail::compare(&back, &other.back)) &&
        (stencilTestEnable == other.stencilTestEnable) &&
        (depthBoundsTestEnable == other.depthBoundsTestEnable) &&
        (minDepthBounds == other.minDepthBounds) &&
        (maxDepthBounds == other.maxDepthBounds);
}

constexpr DepthBoundsState::DepthBoundsState(const DepthStencilState& state,
    float minDepthBounds, float maxDepthBounds):
    DepthStencilState(state.depthCompareOp, state.depthWriteEnable)
{
    front = state.front;
    back = state.back;
    depthBoundsTestEnable = VK_TRUE;
    this->minDepthBounds = minDepthBounds;
    this->maxDepthBounds = maxDepthBounds;
}
} // namespace magma
