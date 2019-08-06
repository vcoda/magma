namespace magma
{
constexpr AdvancedColorBlendAttachmentState::AdvancedColorBlendAttachmentState(VkBlendOp advancedBlendOp,
    VkColorComponentFlags colorWriteMask /* R, G, B, A */):
    VkPipelineColorBlendAttachmentState{}
{
    blendEnable = VK_TRUE;
    srcColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendOp = advancedBlendOp;
    srcAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    alphaBlendOp = advancedBlendOp;
    this->colorWriteMask = colorWriteMask;
}

constexpr size_t AdvancedColorBlendAttachmentState::hash() const
{
    return internal::hashArgs(
        blendEnable,
        srcColorBlendFactor,
        dstColorBlendFactor,
        colorBlendOp,
        srcAlphaBlendFactor,
        dstAlphaBlendFactor,
        alphaBlendOp,
        colorWriteMask);
}

constexpr bool AdvancedColorBlendAttachmentState::operator==(const AdvancedColorBlendAttachmentState& other) const
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
