namespace magma
{
inline std::size_t StencilOpState::hash() const
{
    return core::hashArgs(
        failOp,
        passOp,
        depthFailOp,
        compareOp,
        compareMask,
        writeMask,
        reference);
}

constexpr bool StencilOpState::operator==(const StencilOpState& other) const noexcept
{
    return (failOp == other.failOp) &&
        (passOp == other.passOp) &&
        (depthFailOp == other.depthFailOp) &&
        (compareOp == other.compareOp) &&
        (compareMask == other.compareMask) &&
        (writeMask == other.writeMask) &&
        (reference == other.reference);
}

constexpr DepthStencilState::DepthStencilState(const VkCompareOp depthCompareOp, const bool depthWriteEnable,
    const StencilOpState& front, const StencilOpState& back) noexcept:
    VkPipelineDepthStencilStateCreateInfo{
        VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
        nullptr, // pNext,
        0, // flags
        MAGMA_BOOLEAN(depthCompareOp != VK_COMPARE_OP_ALWAYS), // depthTestEnable
        MAGMA_BOOLEAN(depthWriteEnable),
        depthCompareOp,
        VK_FALSE, // depthBoundsTestEnable
        MAGMA_BOOLEAN((front.compareOp != VK_COMPARE_OP_ALWAYS) || (back.compareOp != VK_COMPARE_OP_ALWAYS)), // stencilTestEnable
        front,
        back,
        0.f, // minDepthBounds
        1.f  // maxDepthBounds
    }
{}

constexpr DepthStencilState::DepthStencilState(const DepthStencilState& state,
    const StencilOpState& front, const StencilOpState& back) noexcept:
    DepthStencilState(state.depthCompareOp, state.depthCompareOp, front, back)
{}

inline std::size_t DepthStencilState::hash() const noexcept
{
    return core::hashArgs(
        sType,
        flags,
        depthTestEnable,
        depthWriteEnable,
        depthCompareOp,
        depthBoundsTestEnable,
        stencilTestEnable,
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
        minDepthBounds,
        maxDepthBounds);
}

constexpr bool DepthStencilState::operator==(const DepthStencilState& other) const noexcept
{
    return (flags == other.flags) &&
        (depthTestEnable == other.depthTestEnable) &&
        (depthWriteEnable == other.depthWriteEnable) &&
        (depthCompareOp == other.depthCompareOp) &&
        (core::compare(&front, &other.front)) &&
        (core::compare(&back, &other.back)) &&
        (stencilTestEnable == other.stencilTestEnable) &&
        (depthBoundsTestEnable == other.depthBoundsTestEnable) &&
        (minDepthBounds == other.minDepthBounds) &&
        (maxDepthBounds == other.maxDepthBounds);
}

constexpr DepthBoundsState::DepthBoundsState(const DepthStencilState& state,
    const float minDepthBounds, const float maxDepthBounds) noexcept:
    DepthStencilState(state)
{
    depthBoundsTestEnable = VK_TRUE;
    this->minDepthBounds = minDepthBounds;
    this->maxDepthBounds = maxDepthBounds;
}
} // namespace magma
