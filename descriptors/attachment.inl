namespace magma
{
constexpr LoadStoreOp::LoadStoreOp(VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp):
    loadOp(loadOp), storeOp(storeOp)
{}

constexpr AttachmentDescription::AttachmentDescription(VkFormat format, uint32_t sampleCount,
    VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp,
    VkAttachmentLoadOp stencilLoadOp, VkAttachmentStoreOp stencilStoreOp,
    VkImageLayout initialLayout, VkImageLayout finalLayout):
    VkAttachmentDescription{}
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
    VkImageLayout initialLayout, VkImageLayout finalLayout):
    AttachmentDescription(format, sampleCount, colorDepthOp.loadOp, colorDepthOp.storeOp,
        stencilOp.loadOp, stencilOp.storeOp, initialLayout, finalLayout)
{}

constexpr AttachmentDescription::AttachmentDescription(const LoadStoreOp& colorDepthOp, const LoadStoreOp& stencilOp,
    VkImageLayout initialLayout, VkImageLayout finalLayout):
    AttachmentDescription(VK_FORMAT_UNDEFINED, VK_SAMPLE_COUNT_1_BIT, colorDepthOp.loadOp, colorDepthOp.storeOp,
        stencilOp.loadOp, stencilOp.storeOp, initialLayout, finalLayout)
{}

constexpr AttachmentDescription::AttachmentDescription(VkFormat format, uint32_t sampleCount, const AttachmentDescription& predefined):
    AttachmentDescription(format, sampleCount, predefined.loadOp, predefined.storeOp,
        predefined.stencilLoadOp, predefined.stencilStoreOp, initialLayout, finalLayout)
{}

constexpr std::size_t AttachmentDescription::hash() const
{
    return detail::hashArgs(
        flags,
        format,
        samples,
        loadOp,
        storeOp,
        stencilLoadOp,
        stencilStoreOp,
        initialLayout,
        finalLayout
    );
}
} // namespace magma
