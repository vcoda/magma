namespace magma
{
constexpr AttachmentDescription::AttachmentDescription(const VkFormat format, const uint32_t sampleCount,
    const LoadStoreOp& colorDepthOp, const LoadStoreOp& stencilOp,
    const VkImageLayout initialLayout, const VkImageLayout finalLayout) noexcept:
    VkAttachmentDescription{
        0, // flags
        format,
        static_cast<VkSampleCountFlagBits>(sampleCount),
        colorDepthOp.load,
        colorDepthOp.store,
        stencilOp.load,
        stencilOp.store,
        initialLayout,
        finalLayout
    }
{
    if ((sampleCount < 1) || (sampleCount > 64))
        samples = VK_SAMPLE_COUNT_1_BIT;
}

constexpr AttachmentDescription::AttachmentDescription(const LoadStoreOp& colorDepthOp, const LoadStoreOp& stencilOp,
    const VkImageLayout initialLayout, const VkImageLayout finalLayout) noexcept:
    AttachmentDescription(VK_FORMAT_UNDEFINED, 1, colorDepthOp, stencilOp, initialLayout, finalLayout)
{}

constexpr hash_t AttachmentDescription::hash() const noexcept
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
