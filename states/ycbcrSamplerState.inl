namespace magma
{
constexpr YcbcrConversionSamplerState::YcbcrConversionSamplerState(const VkSamplerYcbcrModelConversionKHR ycbcrModel,
    const VkSamplerYcbcrRangeKHR ycbcrRange, const VkFilter chromaFilter) noexcept:
    VkSamplerYcbcrConversionCreateInfoKHR{
        VK_STRUCTURE_TYPE_SAMPLER_YCBCR_CONVERSION_CREATE_INFO_KHR,
        nullptr, // pNext
        VK_FORMAT_UNDEFINED, // format
        ycbcrModel,
        ycbcrRange,
        {   // components
            VK_COMPONENT_SWIZZLE_IDENTITY,
            VK_COMPONENT_SWIZZLE_IDENTITY,
            VK_COMPONENT_SWIZZLE_IDENTITY,
            VK_COMPONENT_SWIZZLE_IDENTITY
        },
        VK_CHROMA_LOCATION_COSITED_EVEN_KHR, // xChromaOffset
        VK_CHROMA_LOCATION_COSITED_EVEN_KHR, // yChromaOffset
        chromaFilter,
        VK_FALSE // forceExplicitReconstruction
    }
{}

constexpr std::size_t YcbcrConversionSamplerState::hash() const noexcept
{
    return core::hashArgs(
        sType,
        format,
        ycbcrModel,
        ycbcrRange,
        components.r,
        components.g,
        components.b,
        components.a,
        xChromaOffset,
        yChromaOffset,
        chromaFilter,
        forceExplicitReconstruction);
}

constexpr bool YcbcrConversionSamplerState::operator==(const YcbcrConversionSamplerState& other) const noexcept
{
    return (format == other.format) &&
        (ycbcrModel == other.ycbcrModel) &&
        (ycbcrRange == other.ycbcrRange) &&
        (components.r == other.components.r) &&
        (components.g == other.components.g) &&
        (components.b == other.components.b) &&
        (components.a == other.components.a) &&
        (xChromaOffset == other.xChromaOffset) &&
        (yChromaOffset == other.yChromaOffset) &&
        (chromaFilter == other.chromaFilter) &&
        (forceExplicitReconstruction == other.forceExplicitReconstruction);
}
} // namespace magma
