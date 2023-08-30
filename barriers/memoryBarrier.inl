namespace magma
{
constexpr MemoryBarrier::MemoryBarrier(const VkAccessFlags srcAccessMask, const VkAccessFlags dstAccessMask) noexcept:
    VkMemoryBarrier{
        VK_STRUCTURE_TYPE_MEMORY_BARRIER,
        nullptr, // pNext
        srcAccessMask,
        dstAccessMask
    }
{}
} // namespace magma
