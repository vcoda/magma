namespace magma
{
constexpr AdvancedColorBlendAttachmentState::AdvancedColorBlendAttachmentState(const VkBlendOp advancedBlendOp,
    const VkColorComponentFlags colorWriteMask /* colorwritemask::rgba */) noexcept:
    VkPipelineColorBlendAttachmentState{
        VK_TRUE, // blendEnable
        VK_BLEND_FACTOR_ZERO, // srcColorBlendFactor
        VK_BLEND_FACTOR_ZERO, // dstColorBlendFactor
        advancedBlendOp, // colorBlendOp
        VK_BLEND_FACTOR_ZERO, // srcAlphaBlendFactor
        VK_BLEND_FACTOR_ZERO, // dstAlphaBlendFactor
        advancedBlendOp, // alphaBlendOp
        colorWriteMask // colorWriteMask
    }
{}

constexpr hash_t AdvancedColorBlendAttachmentState::hash() const noexcept
{
    return core::hashArgs(
        blendEnable,
        srcColorBlendFactor,
        dstColorBlendFactor,
        colorBlendOp,
        srcAlphaBlendFactor,
        dstAlphaBlendFactor,
        alphaBlendOp,
        colorWriteMask);
}

constexpr bool AdvancedColorBlendAttachmentState::operator==(const AdvancedColorBlendAttachmentState& other) const noexcept
{
    return (blendEnable == other.blendEnable) &&
        (srcColorBlendFactor == other.srcColorBlendFactor) &&
        (dstColorBlendFactor == other.dstColorBlendFactor) &&
        (colorBlendOp == other.colorBlendOp) &&
        (srcAlphaBlendFactor == other.srcAlphaBlendFactor) &&
        (dstAlphaBlendFactor == other.dstAlphaBlendFactor) &&
        (alphaBlendOp == other.alphaBlendOp) &&
        (colorWriteMask == other.colorWriteMask);
}
} // namespace magma
