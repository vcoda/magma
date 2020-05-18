namespace magma
{
constexpr LoadStoreOp::LoadStoreOp(const VkAttachmentLoadOp loadOp, const VkAttachmentStoreOp storeOp) noexcept:
    loadOp(loadOp),
    storeOp(storeOp)
{}

constexpr AttachmentDescription::AttachmentDescription(const VkFormat format, const uint32_t sampleCount,
    const VkAttachmentLoadOp loadOp, const VkAttachmentStoreOp storeOp,
    const VkAttachmentLoadOp stencilLoadOp, const VkAttachmentStoreOp stencilStoreOp,
    const VkImageLayout initialLayout, const VkImageLayout finalLayout) noexcept:
    VkAttachmentDescription{
        0, // flags
        format,
        VK_SAMPLE_COUNT_1_BIT, // samples
        loadOp,
        storeOp,
        stencilLoadOp,
        stencilStoreOp,
        initialLayout,
        finalLayout
    }
{
    switch (sampleCount)
    {
    case 1: samples = VK_SAMPLE_COUNT_1_BIT; break;
    case 2: samples = VK_SAMPLE_COUNT_2_BIT; break;
    case 4: samples = VK_SAMPLE_COUNT_4_BIT; break;
    case 8: samples = VK_SAMPLE_COUNT_8_BIT; break;
    case 16: samples = VK_SAMPLE_COUNT_16_BIT; break;
    case 32: samples = VK_SAMPLE_COUNT_32_BIT; break;
    case 64: samples = VK_SAMPLE_COUNT_64_BIT; break;
    default: samples = VK_SAMPLE_COUNT_1_BIT;
    }
}

constexpr AttachmentDescription::AttachmentDescription(const VkFormat format, const uint32_t sampleCount,
    const LoadStoreOp& colorDepthOp, const LoadStoreOp& stencilOp,
    const VkImageLayout initialLayout, const VkImageLayout finalLayout) noexcept:
    AttachmentDescription(format, sampleCount, colorDepthOp.loadOp, colorDepthOp.storeOp,
        stencilOp.loadOp, stencilOp.storeOp, initialLayout, finalLayout)
{}

constexpr AttachmentDescription::AttachmentDescription(const LoadStoreOp& colorDepthOp, const LoadStoreOp& stencilOp,
    const VkImageLayout initialLayout, const VkImageLayout finalLayout) noexcept:
    AttachmentDescription(VK_FORMAT_UNDEFINED, VK_SAMPLE_COUNT_1_BIT, colorDepthOp.loadOp, colorDepthOp.storeOp,
        stencilOp.loadOp, stencilOp.storeOp, initialLayout, finalLayout)
{}

constexpr AttachmentDescription::AttachmentDescription(const VkFormat format, const uint32_t sampleCount, const AttachmentDescription& predefined) noexcept:
    AttachmentDescription(format, sampleCount, predefined.loadOp, predefined.storeOp,
        predefined.stencilLoadOp, predefined.stencilStoreOp, initialLayout, finalLayout)
{}

inline std::size_t AttachmentDescription::hash() const
{
    return core::hashArgs(
        flags,
        format,
        samples,
        loadOp,
        storeOp,
        stencilLoadOp,
        stencilStoreOp,
        initialLayout,
        finalLayout);
}
} // namespace magma
