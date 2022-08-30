namespace magma
{
inline ImagelessFramebuffer::AttachmentImageInfo::AttachmentImageInfo(VkImageUsageFlags usage,
    uint32_t width, uint32_t height, uint32_t layerCount, const std::vector<VkFormat> viewFormats,
    VkImageCreateFlags flags /* 0 */):
    VkFramebufferAttachmentImageInfoKHR{
        VK_STRUCTURE_TYPE_FRAMEBUFFER_ATTACHMENT_IMAGE_INFO_KHR,
        nullptr, // pNext
        flags,
        usage,
        width,
        height,
        layerCount,
        MAGMA_COUNT(viewFormats),
        viewFormats.data()
    },
    viewFormats(std::move(viewFormats))
{}

inline ImagelessFramebuffer::AttachmentImageInfo::AttachmentImageInfo(const AttachmentImageInfo& other):
    viewFormats(other.viewFormats)
{
    sType = other.sType;
    pNext = other.pNext;
    flags = other.flags;
    usage = other.usage;
    width = other.width;
    height = other.height;
    layerCount = other.layerCount;
    viewFormatCount = other.viewFormatCount;
    pViewFormats = viewFormats.data();
}

inline ImagelessFramebuffer::AttachmentImageInfo& ImagelessFramebuffer::AttachmentImageInfo::operator=(const AttachmentImageInfo& other)
{
    if (this != &other)
    {
        sType = other.sType;
        pNext = other.pNext;
        flags = other.flags;
        usage = other.usage;
        width = other.width;
        height = other.height;
        layerCount = other.layerCount;
        viewFormatCount = other.viewFormatCount;
        viewFormats = other.viewFormats;
        pViewFormats = viewFormats.data();
    }
    return *this;
}
} // namespace magma
