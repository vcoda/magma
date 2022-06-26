namespace magma
{
constexpr DepthComparisonSamplerState::DepthComparisonSamplerState(const VkFilter magFilter, const VkFilter minFilter, const VkCompareOp compareOp_) noexcept:
    SamplerState(magFilter, minFilter, VK_SAMPLER_MIPMAP_MODE_NEAREST, VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER)
{
    compareEnable = VK_TRUE;
    compareOp = compareOp_;
    borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
}
} // namespace magma
