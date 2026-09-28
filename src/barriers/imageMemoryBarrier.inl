namespace magma
{
constexpr ImageMemoryBarrier::ImageMemoryBarrier(VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask) noexcept:
    VkImageMemoryBarrier{
        VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        nullptr, // pNext
        srcAccessMask,
        dstAccessMask,
        VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_UNDEFINED,
        VK_QUEUE_FAMILY_IGNORED,
        VK_QUEUE_FAMILY_IGNORED,
        VK_NULL_HANDLE, // image
        VkImageSubresourceRange{
            VK_IMAGE_ASPECT_NONE,
            0,
            1,
            0,
            1
        }
    }
{}
} // namespace magma
