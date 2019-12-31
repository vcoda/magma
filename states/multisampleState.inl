namespace magma
{
constexpr MultisampleState::MultisampleState(VkSampleCountFlagBits rasterizationSamples):
    VkPipelineMultisampleStateCreateInfo{}
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    pNext = nullptr;
    flags = 0;
    this->rasterizationSamples = rasterizationSamples;
    sampleShadingEnable = VK_FALSE;
    minSampleShading = 0.f;
    pSampleMask = nullptr;
    alphaToCoverageEnable = VK_FALSE;
    alphaToOneEnable = VK_FALSE;
}

constexpr MultisampleState::MultisampleState(uint32_t sampleCount):
    VkPipelineMultisampleStateCreateInfo{}
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    pNext = nullptr;
    flags = 0;
    switch (sampleCount)
    {
    case 1: rasterizationSamples = VK_SAMPLE_COUNT_1_BIT; break;
    case 2: rasterizationSamples = VK_SAMPLE_COUNT_2_BIT; break;
    case 4: rasterizationSamples = VK_SAMPLE_COUNT_4_BIT; break;
    case 8: rasterizationSamples = VK_SAMPLE_COUNT_8_BIT; break;
    case 16: rasterizationSamples = VK_SAMPLE_COUNT_16_BIT; break;
    case 32: rasterizationSamples = VK_SAMPLE_COUNT_32_BIT; break;
    case 64: rasterizationSamples = VK_SAMPLE_COUNT_64_BIT; break;
    default:
        rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    }
    sampleShadingEnable = VK_FALSE;
    minSampleShading = 0.f;
    pSampleMask = nullptr;
    alphaToCoverageEnable = VK_FALSE;
    alphaToOneEnable = VK_FALSE;
}

constexpr std::size_t MultisampleState::hash() const
{
    std::size_t hash = detail::hashArgs(
        sType,
        flags,
        rasterizationSamples,
        sampleShadingEnable,
        minSampleShading,
        alphaToCoverageEnable,
        alphaToOneEnable);
    if (pSampleMask)
    {
        detail::hashCombine(hash, detail::hash(pSampleMask[0]));
        if (rasterizationSamples > VK_SAMPLE_COUNT_32_BIT)
            detail::hashCombine(hash, detail::hash(pSampleMask[1]));
    }
    return hash;
}

constexpr bool MultisampleState::operator==(const MultisampleState& other) const
{
    return (flags == other.flags) &&
        (rasterizationSamples == other.rasterizationSamples) &&
        (sampleShadingEnable == other.sampleShadingEnable) &&
        (minSampleShading == other.minSampleShading) &&
        (detail::compareArrays(pSampleMask, other.pSampleMask, rasterizationSamples > VK_SAMPLE_COUNT_32_BIT ? 2 : 1)) &&
        (alphaToCoverageEnable == other.alphaToCoverageEnable) &&
        (alphaToOneEnable == other.alphaToOneEnable);
}

constexpr MultisampleShadingState::MultisampleShadingState(const MultisampleState& state,
    float minSampleShading /* 1 */):
    MultisampleState(state.rasterizationSamples)
{
    sampleShadingEnable = VK_TRUE;
    this->minSampleShading = minSampleShading;
}
} // namespace magma
