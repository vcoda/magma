namespace magma
{
constexpr ColorBlendAttachmentState::ColorBlendAttachmentState(
    VkColorComponentFlags colorWriteMask /* R, G, B, A */)
{
    blendEnable = VK_FALSE;
    srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
    dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendOp = VK_BLEND_OP_ADD;
    srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    alphaBlendOp = VK_BLEND_OP_ADD;
    this->colorWriteMask = colorWriteMask;
}

constexpr ColorBlendAttachmentState::ColorBlendAttachmentState(
    VkBlendFactor srcBlendFactor, VkBlendFactor dstBlendFactor, VkBlendOp blendOp,
    VkColorComponentFlags colorWriteMask /* R, G, B, A */)
{
    blendEnable = VK_TRUE;
    srcColorBlendFactor = srcBlendFactor;
    dstColorBlendFactor = dstBlendFactor;
    colorBlendOp = blendOp;
    srcAlphaBlendFactor = srcBlendFactor;
    dstAlphaBlendFactor = dstBlendFactor;
    alphaBlendOp = blendOp;
    this->colorWriteMask = colorWriteMask;
}

constexpr ColorBlendAttachmentState::ColorBlendAttachmentState(
    VkBlendFactor srcColorBlendFactor, VkBlendFactor dstColorBlendFactor, VkBlendOp colorBlendOp,
    VkBlendFactor srcAlphaBlendFactor, VkBlendFactor dstAlphaBlendFactor, VkBlendOp alphaBlendOp,
    VkColorComponentFlags colorWriteMask /* R, G, B, A */)
{
    blendEnable = VK_TRUE;
    this->srcColorBlendFactor = srcColorBlendFactor;
    this->dstColorBlendFactor = dstColorBlendFactor;
    this->colorBlendOp = colorBlendOp;
    this->srcAlphaBlendFactor = srcAlphaBlendFactor;
    this->dstAlphaBlendFactor = dstAlphaBlendFactor;
    this->alphaBlendOp = alphaBlendOp;
    this->colorWriteMask = colorWriteMask;
}

constexpr size_t ColorBlendAttachmentState::hash() const
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

constexpr bool ColorBlendAttachmentState::operator==(const ColorBlendAttachmentState& other) const
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

constexpr ColorBlendState::ColorBlendState()
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    pNext = nullptr;
    flags = 0;
    logicOpEnable = VK_FALSE;
    logicOp = VK_LOGIC_OP_CLEAR;
    attachmentCount = 0;
    pAttachments = nullptr;
    blendConstants[0] = 0.f;
    blendConstants[1] = 0.f;
    blendConstants[2] = 0.f;
    blendConstants[3] = 0.f;
}

constexpr ColorBlendState::ColorBlendState(const ColorBlendAttachmentState& attachment,
    bool logicOpEnable /* false */,
    VkLogicOp logicOp /* VK_LOGIC_OP_CLEAR */,
    const std::initializer_list<float>& blendConstants /* {1, 1, 1, 1} */)
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    pNext = nullptr;
    flags = 0;
    this->logicOpEnable = MAGMA_BOOLEAN(logicOpEnable);
    this->logicOp = logicOp;
    attachmentCount = 1;
    pAttachments = &attachment;
    const auto c = blendConstants.begin();
    this->blendConstants[0] = c[0];
    this->blendConstants[1] = c[1];
    this->blendConstants[2] = c[2];
    this->blendConstants[3] = c[3];
}

constexpr size_t ColorBlendState::hash() const
{
    size_t hash = internal::hashArgs(
        flags,
        logicOpEnable,
        logicOp,
        attachmentCount);
    internal::hashCombine(hash, internal::hashArgs(
        pAttachments->blendEnable,
        pAttachments->srcColorBlendFactor,
        pAttachments->dstColorBlendFactor,
        pAttachments->colorBlendOp,
        pAttachments->srcAlphaBlendFactor,
        pAttachments->dstAlphaBlendFactor,
        pAttachments->alphaBlendOp,
        pAttachments->colorWriteMask));
    internal::hashCombine(hash, internal::hashArray(blendConstants, 4));
    return hash;
}

constexpr bool ColorBlendState::operator==(const ColorBlendState& other) const
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

constexpr ColorLogicOpState::ColorLogicOpState(const ColorBlendAttachmentState& attachment, VkLogicOp logicOp):
    ColorBlendState(attachment, true, logicOp)
{}
} // namespace magma
