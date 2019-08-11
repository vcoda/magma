namespace magma
{
constexpr SamplerState::SamplerState(VkFilter magFilter, VkFilter minFilter,
    VkSamplerMipmapMode mipmapMode, VkSamplerAddressMode addressMode):
    magFilter(magFilter),
    minFilter(minFilter),
    mipmapMode(mipmapMode),
    addressMode(addressMode),
    anisotropyEnable(false),
    maxAnisotropy(1.f)
{}

constexpr std::size_t SamplerState::hash() const
{
    return internal::hashArgs(
        magFilter,
        minFilter,
        mipmapMode,
        addressMode,
        anisotropyEnable,
        maxAnisotropy);
}

constexpr bool SamplerState::operator==(const SamplerState& other) const
{
    return (magFilter == other.magFilter) &&
        (minFilter == other.minFilter) &&
        (mipmapMode == other.mipmapMode) &&
        (addressMode == other.addressMode) &&
        (anisotropyEnable == other.anisotropyEnable) &&
        (maxAnisotropy == other.maxAnisotropy);
}

constexpr AnisotropicSamplerState::AnisotropicSamplerState(VkFilter magFilter, VkFilter minFilter, VkSamplerAddressMode addressMode,
    float maxAnisotropy /* std::numeric_limits<float>::max() */ ):
    SamplerState(magFilter, minFilter, VK_SAMPLER_MIPMAP_MODE_LINEAR, addressMode)
{
    anisotropyEnable = true;
    // If anisotropyEnable is VK_TRUE, maxAnisotropy must be between 
    // 1.0 and VkPhysicalDeviceLimits::maxSamplerAnisotropy, inclusive.
    if (maxAnisotropy < 1.f)
        maxAnisotropy = 1.f;
    this->maxAnisotropy = maxAnisotropy;
}

constexpr DepthSamplerState::DepthSamplerState(VkFilter magFilter, VkFilter minFilter, VkCompareOp compareOp):
    magFilter(magFilter),
    minFilter(minFilter),
    compareOp(compareOp)
{}

constexpr std::size_t DepthSamplerState::hash() const
{
    return internal::hashArgs(
        magFilter,
        minFilter,
        compareOp);
}

constexpr bool DepthSamplerState::operator==(const DepthSamplerState& other) const
{
    return (magFilter == other.magFilter) &&
        (minFilter == other.minFilter) &&
        (compareOp == other.compareOp);
}
} // namespace magma
