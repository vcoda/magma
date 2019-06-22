namespace magma
{
constexpr LoadStoreOp::LoadStoreOp(VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp) noexcept:
    loadOp(loadOp), storeOp(storeOp) 
{}

constexpr AttachmentDescription::AttachmentDescription(VkFormat format, uint32_t sampleCount,
    VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp,
    VkAttachmentLoadOp stencilLoadOp, VkAttachmentStoreOp stencilStoreOp,
    VkImageLayout initialLayout, VkImageLayout finalLayout) noexcept
{
    flags = 0;
    this->format = format;
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
    this->loadOp = loadOp;
    this->storeOp = storeOp;
    this->stencilLoadOp = stencilLoadOp;
    this->stencilStoreOp = stencilStoreOp;
    this->initialLayout = initialLayout;
    this->finalLayout = finalLayout;
}

constexpr AttachmentDescription::AttachmentDescription(VkFormat format, uint32_t sampleCount, 
    const LoadStoreOp& colorDepthOp, const LoadStoreOp& stencilOp,
    VkImageLayout initialLayout, VkImageLayout finalLayout) noexcept:
    AttachmentDescription(format, sampleCount, colorDepthOp.loadOp, colorDepthOp.storeOp,
        stencilOp.loadOp, stencilOp.storeOp, initialLayout, finalLayout)
{}

constexpr AttachmentDescription::AttachmentDescription(const LoadStoreOp& colorDepthOp, const LoadStoreOp& stencilOp,
    VkImageLayout initialLayout, VkImageLayout finalLayout) noexcept:
    AttachmentDescription(VK_FORMAT_UNDEFINED, VK_SAMPLE_COUNT_1_BIT, colorDepthOp.loadOp, colorDepthOp.storeOp,
        stencilOp.loadOp, stencilOp.storeOp, initialLayout, finalLayout)
{}

constexpr AttachmentDescription::AttachmentDescription(VkFormat format, uint32_t sampleCount, 
    const AttachmentDescription& prefab) noexcept:
    AttachmentDescription(format, sampleCount, prefab.loadOp, prefab.storeOp,
        prefab.stencilLoadOp, prefab.stencilStoreOp, initialLayout, finalLayout)
{}
} // namespace magma
