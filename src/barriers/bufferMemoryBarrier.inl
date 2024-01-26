namespace magma
{
constexpr BufferMemoryBarrier::BufferMemoryBarrier(const VkAccessFlags srcAccessMask, const VkAccessFlags dstAccessMask,
    const VkDeviceSize offset /* 0 */, const VkDeviceSize size /* VK_WHOLE_SIZE */) noexcept:
    VkBufferMemoryBarrier{
        VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER,
        nullptr, // pNext
        srcAccessMask,
        dstAccessMask,
        VK_QUEUE_FAMILY_IGNORED, // srcQueueFamilyIndex
        VK_QUEUE_FAMILY_IGNORED, // dstQueueFamilyIndex
        VK_NULL_HANDLE, // buffer
        offset,
        size
    }
{}
} // namespace magma
