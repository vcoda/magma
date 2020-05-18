namespace magma
{
constexpr MultisampleState::MultisampleState(const VkSampleCountFlagBits rasterizationSamples) noexcept:
    VkPipelineMultisampleStateCreateInfo{
        VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        nullptr, // pNext
        0, // flags
        rasterizationSamples,
        VK_FALSE, // sampleShadingEnable
        0.f, // minSampleShading
        nullptr, // pSampleMask
        VK_FALSE, // alphaToCoverageEnable
        VK_FALSE, // alphaToOneEnable
    }
{}

constexpr MultisampleState::MultisampleState(const uint32_t sampleCount) noexcept:
    VkPipelineMultisampleStateCreateInfo{
        VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        nullptr, // pNext
        0, // flags
        VK_SAMPLE_COUNT_1_BIT,
        VK_FALSE, // sampleShadingEnable
        0.f, // minSampleShading
        nullptr, // pSampleMask
        VK_FALSE, // alphaToCoverageEnable
        VK_FALSE, // alphaToOneEnable
    }
{
    switch (sampleCount)
    {
    case 1: rasterizationSamples = VK_SAMPLE_COUNT_1_BIT; break;
    case 2: rasterizationSamples = VK_SAMPLE_COUNT_2_BIT; break;
    case 4: rasterizationSamples = VK_SAMPLE_COUNT_4_BIT; break;
    case 8: rasterizationSamples = VK_SAMPLE_COUNT_8_BIT; break;
    case 16: rasterizationSamples = VK_SAMPLE_COUNT_16_BIT; break;
    case 32: rasterizationSamples = VK_SAMPLE_COUNT_32_BIT; break;
    case 64: rasterizationSamples = VK_SAMPLE_COUNT_64_BIT; break;
    default: rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    }
}

inline std::size_t MultisampleState::hash() const
{
    std::size_t hash = core::hashArgs(
        sType,
        flags,
        rasterizationSamples,
        sampleShadingEnable,
        minSampleShading,
        alphaToCoverageEnable,
        alphaToOneEnable);
    if (pSampleMask)
    {
        core::hashCombine(hash, core::hash(pSampleMask[0]));
        if (rasterizationSamples > VK_SAMPLE_COUNT_32_BIT)
            core::hashCombine(hash, core::hash(pSampleMask[1]));
    }
    return hash;
}

constexpr bool MultisampleState::operator==(const MultisampleState& other) const
{
    return (flags == other.flags) &&
        (rasterizationSamples == other.rasterizationSamples) &&
        (sampleShadingEnable == other.sampleShadingEnable) &&
        (minSampleShading == other.minSampleShading) &&
        (core::compareArrays(pSampleMask, other.pSampleMask, rasterizationSamples > VK_SAMPLE_COUNT_32_BIT ? 2 : 1)) &&
        (alphaToCoverageEnable == other.alphaToCoverageEnable) &&
        (alphaToOneEnable == other.alphaToOneEnable);
}

constexpr MultisampleShadingState::MultisampleShadingState(const MultisampleState& state, const float minSampleShading /* 1 */) noexcept:
    MultisampleState(state)
{
    sampleShadingEnable = VK_TRUE;
    this->minSampleShading = minSampleShading;
}
} // namespace magma
