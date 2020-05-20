namespace magma
{
constexpr SamplerState::SamplerState(const VkFilter magFilter, const VkFilter minFilter,
    const VkSamplerMipmapMode mipmapMode, const VkSamplerAddressMode addressMode) noexcept:
    magFilter(magFilter),
    minFilter(minFilter),
    mipmapMode(mipmapMode),
    addressMode(addressMode),
    anisotropyEnable(false),
    maxAnisotropy(1.f)
{}

inline std::size_t SamplerState::hash() const noexcept
{
    return core::hashArgs(
        magFilter,
        minFilter,
        mipmapMode,
        addressMode,
        anisotropyEnable,
        maxAnisotropy);
}

constexpr bool SamplerState::operator==(const SamplerState& other) const noexcept
{
    return (magFilter == other.magFilter) &&
        (minFilter == other.minFilter) &&
        (mipmapMode == other.mipmapMode) &&
        (addressMode == other.addressMode) &&
        (anisotropyEnable == other.anisotropyEnable) &&
        (maxAnisotropy == other.maxAnisotropy);
}

constexpr AnisotropicSamplerState::AnisotropicSamplerState(const VkFilter magFilter, const VkFilter minFilter, const VkSamplerAddressMode addressMode,
    const float maxAnisotropy /* std::numeric_limits<float>::max() */) noexcept:
    SamplerState(magFilter, minFilter, VK_SAMPLER_MIPMAP_MODE_LINEAR, addressMode)
{
    anisotropyEnable = true;
    // If anisotropyEnable is VK_TRUE, maxAnisotropy must be between
    // 1.0 and VkPhysicalDeviceLimits::maxSamplerAnisotropy, inclusive.
    this->maxAnisotropy = maxAnisotropy;
    if (this->maxAnisotropy < 1.f)
        this->maxAnisotropy = 1.f;
}

constexpr DepthSamplerState::DepthSamplerState(const VkFilter magFilter, const VkFilter minFilter, const VkCompareOp compareOp) noexcept:
    magFilter(magFilter),
    minFilter(minFilter),
    compareOp(compareOp)
{}

inline std::size_t DepthSamplerState::hash() const noexcept
{
    return core::hashArgs(
        magFilter,
        minFilter,
        compareOp);
}

constexpr bool DepthSamplerState::operator==(const DepthSamplerState& other) const noexcept
{
    return (magFilter == other.magFilter) &&
        (minFilter == other.minFilter) &&
        (compareOp == other.compareOp);
}
} // namespace magma
