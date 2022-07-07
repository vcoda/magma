namespace magma
{
constexpr ColorBlendAttachmentState::ColorBlendAttachmentState(const VkColorComponentFlags colorWriteMask /* colormask::rgba */) noexcept:
    VkPipelineColorBlendAttachmentState{
        VK_FALSE, // blendEnable
        VK_BLEND_FACTOR_ONE, // srcColorBlendFactor
        VK_BLEND_FACTOR_ZERO, // dstColorBlendFactor
        VK_BLEND_OP_ADD, // colorBlendOp
        VK_BLEND_FACTOR_ONE, // srcAlphaBlendFactor
        VK_BLEND_FACTOR_ZERO, // dstAlphaBlendFactor
        VK_BLEND_OP_ADD, // alphaBlendOp
        colorWriteMask // colorWriteMask
    }
{}

constexpr ColorBlendAttachmentState::ColorBlendAttachmentState(const VkBlendFactor srcBlendFactor, const VkBlendFactor dstBlendFactor, const VkBlendOp blendOp,
    const VkColorComponentFlags colorWriteMask /* colormask::rgba */) noexcept:
    VkPipelineColorBlendAttachmentState{
        VK_TRUE, // blendEnable
        srcBlendFactor, // srcColorBlendFactor
        dstBlendFactor, // dstColorBlendFactor
        blendOp, // colorBlendOp
        srcBlendFactor, // srcAlphaBlendFactor
        dstBlendFactor, // dstAlphaBlendFactor
        blendOp, // alphaBlendOp
        colorWriteMask // colorWriteMask
    }
{}

constexpr ColorBlendAttachmentState::ColorBlendAttachmentState(
    const VkBlendFactor srcColorBlendFactor, const VkBlendFactor dstColorBlendFactor, const VkBlendOp colorBlendOp,
    const VkBlendFactor srcAlphaBlendFactor, const VkBlendFactor dstAlphaBlendFactor, const VkBlendOp alphaBlendOp,
    const VkColorComponentFlags colorWriteMask /* colormask::rgba */) noexcept:
    VkPipelineColorBlendAttachmentState{
        VK_TRUE, // blendEnable
        srcColorBlendFactor,
        dstColorBlendFactor,
        colorBlendOp,
        srcAlphaBlendFactor,
        dstAlphaBlendFactor,
        alphaBlendOp,
        colorWriteMask
    }
{}

constexpr hash_t ColorBlendAttachmentState::hash() const noexcept
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

constexpr bool ColorBlendAttachmentState::operator==(const ColorBlendAttachmentState& other) const noexcept
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
