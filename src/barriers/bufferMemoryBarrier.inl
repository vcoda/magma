namespace magma
{
constexpr BufferMemoryBarrier::BufferMemoryBarrier(const VkAccessFlags srcAccessMask, const VkAccessFlags dstAccessMask) noexcept:
    VkBufferMemoryBarrier{
        VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER,
        nullptr, // pNext
        srcAccessMask,
        dstAccessMask,
        VK_QUEUE_FAMILY_IGNORED,
        VK_QUEUE_FAMILY_IGNORED,
        VK_NULL_HANDLE, // buffer
        0, // offset
        VK_WHOLE_SIZE
    }
{}

inline BufferMemoryBarrier::BufferMemoryBarrier(std::shared_ptr<const Buffer> buffer,
    VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask,
    VkDeviceSize offset /* 0 */, VkDeviceSize size /* VK_WHOLE_SIZE */) noexcept:
    VkBufferMemoryBarrier{
        VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER,
        nullptr, // pNext
        srcAccessMask,
        dstAccessMask,
        VK_QUEUE_FAMILY_IGNORED,
        VK_QUEUE_FAMILY_IGNORED,
        *buffer,
        offset,
        size
    }
{}

inline BufferMemoryBarrier::BufferMemoryBarrier(std::shared_ptr<const Buffer> buffer_, const BufferMemoryBarrier& barrier) noexcept:
    VkBufferMemoryBarrier{barrier}
{
    buffer = *buffer_;
}

inline BufferMemoryBarrier::BufferMemoryBarrier(std::shared_ptr<const Buffer> buffer, const BufferMemoryBarrier& barrier,
    VkDeviceSize offset, VkDeviceSize size) noexcept:
    VkBufferMemoryBarrier{
        barrier.sType,
        barrier.pNext,
        barrier.srcAccessMask,
        barrier.dstAccessMask,
        barrier.srcQueueFamilyIndex,
        barrier.dstQueueFamilyIndex,
        *buffer,
        offset,
        size
    }
{}
} // namespace magma
