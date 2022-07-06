namespace magma
{
constexpr ColorBlendAttachmentState::ColorBlendAttachmentState(
    const VkColorComponentFlags colorWriteMask /* colorwritemask::rgba */) noexcept:
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

constexpr ColorBlendAttachmentState::ColorBlendAttachmentState(
    const VkBlendFactor srcBlendFactor, const VkBlendFactor dstBlendFactor, const VkBlendOp blendOp,
    const VkColorComponentFlags colorWriteMask /* colorwritemask::rgba */) noexcept:
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
    const VkColorComponentFlags colorWriteMask /* colorwritemask::rgba */) noexcept:
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

constexpr ColorBlendState::ColorBlendState() noexcept:
    VkPipelineColorBlendStateCreateInfo{
        VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        nullptr, // pNext
        0, // flags
        VK_FALSE, // logicOpEnable
        VK_LOGIC_OP_CLEAR, // logicOp
        0, // attachmentCount
        nullptr, // pAttachments
        {0.f, 0.f, 0.f, 0.f} // blendConstants
    }
{}

constexpr ColorBlendState::ColorBlendState(const ColorBlendAttachmentState& attachment,
    bool logicOpEnable /* false */,
    VkLogicOp logicOp /* VK_LOGIC_OP_CLEAR */,
    const std::initializer_list<float>& blendConstants /* {1, 1, 1, 1} */) noexcept:
    VkPipelineColorBlendStateCreateInfo{
        VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        nullptr, // pNext
        0, // flags
        MAGMA_BOOLEAN(logicOpEnable),
        logicOp,
        1, // attachmentCount
        &attachment, // pAttachments
        {0.f, 0.f, 0.f, 0.f} // blendConstants
    }
{
    const auto c = blendConstants.begin();
    this->blendConstants[0] = c[0];
    this->blendConstants[1] = c[1];
    this->blendConstants[2] = c[2];
    this->blendConstants[3] = c[3];
}

constexpr hash_t ColorBlendState::hash() const noexcept
{
    return core::hashArgs(
        sType,
        flags,
        logicOpEnable,
        logicOp,
        attachmentCount,
        pAttachments->blendEnable,
        pAttachments->srcColorBlendFactor,
        pAttachments->dstColorBlendFactor,
        pAttachments->colorBlendOp,
        pAttachments->srcAlphaBlendFactor,
        pAttachments->dstAlphaBlendFactor,
        pAttachments->alphaBlendOp,
        pAttachments->colorWriteMask,
        blendConstants[0],
        blendConstants[1],
        blendConstants[2],
        blendConstants[3]);
}

constexpr bool ColorBlendState::operator==(const ColorBlendState& other) const noexcept
{
    return (flags == other.flags) &&
        (logicOpEnable == other.logicOpEnable) &&
        (logicOp == other.logicOp) &&
        (attachmentCount == other.attachmentCount) &&
        (pAttachments->blendEnable == other.pAttachments->blendEnable) &&
        (pAttachments->srcColorBlendFactor == other.pAttachments->srcColorBlendFactor) &&
        (pAttachments->dstColorBlendFactor == other.pAttachments->dstColorBlendFactor) &&
        (pAttachments->colorBlendOp == other.pAttachments->colorBlendOp) &&
        (pAttachments->srcAlphaBlendFactor == other.pAttachments->srcAlphaBlendFactor) &&
        (pAttachments->dstAlphaBlendFactor == other.pAttachments->dstAlphaBlendFactor) &&
        (pAttachments->alphaBlendOp == other.pAttachments->alphaBlendOp) &&
        (pAttachments->colorWriteMask == other.pAttachments->colorWriteMask) &&
        (blendConstants[0] == other.blendConstants[0]) &&
        (blendConstants[1] == other.blendConstants[1]) &&
        (blendConstants[2] == other.blendConstants[2]) &&
        (blendConstants[3] == other.blendConstants[3]);
}

constexpr ColorLogicOpState::ColorLogicOpState(const ColorBlendAttachmentState& attachment, const VkLogicOp logicOp) noexcept:
    ColorBlendState(attachment, true, logicOp)
{}
} // namespace magma
