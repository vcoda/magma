namespace magma
{
constexpr StencilOpState::StencilOpState(const VkStencilOp failOp, const VkStencilOp passOp, const VkStencilOp depthFailOp, const VkCompareOp compareOp,
    const uint32_t compareMask /* 0 */, const uint32_t writeMask /* 0 */, const uint32_t reference /* 0 */) noexcept:
    VkStencilOpState{
        failOp,
        passOp,
        depthFailOp,
        compareOp,
        compareMask,
        writeMask,
        reference
    }
{}
} // namespace magma
