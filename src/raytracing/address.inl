namespace magma
{
#ifdef VK_KHR_acceleration_structure
inline VkDeviceOrHostAddressConstKHR address(VkDeviceAddress deviceAddress) noexcept
{
    VkDeviceOrHostAddressConstKHR addr;
    addr.deviceAddress = deviceAddress;
    return addr;
}

inline VkDeviceOrHostAddressConstKHR address(const void *hostAddress) noexcept
{
    VkDeviceOrHostAddressConstKHR addr;
    addr.hostAddress = hostAddress;
    return addr;
}

template<class Buffer>
inline VkDeviceOrHostAddressConstKHR address(const Buffer& buffer) noexcept
{
    VkDeviceOrHostAddressConstKHR addr;
    addr.deviceAddress = buffer ? buffer->getDeviceAddress() : MAGMA_NULL;
    return addr;
}

inline VkDeviceAddress align(std::size_t alignment, VkDeviceSize sizeOfBufferToBeAligned,
    VkDeviceAddress address, VkDeviceSize& sizeOfBufferLeftToOperate) noexcept
{
    MAGMA_ASSERT(sizeOfBufferLeftToOperate);
    std::size_t size = static_cast<std::size_t>(sizeOfBufferToBeAligned);
    void *ptr = reinterpret_cast<void *>(address);
    std::size_t space = static_cast<std::size_t>(sizeOfBufferLeftToOperate);
    void *aligned = std::align(alignment, size, ptr, space);
    MAGMA_ASSERT(aligned);
    if (!aligned)
        return MAGMA_NULL;
    VkDeviceAddress alignedAddress = static_cast<VkDeviceAddress>(reinterpret_cast<uintptr_t>(aligned));
    MAGMA_ASSERT(alignedAddress % alignment == 0);
    sizeOfBufferLeftToOperate = static_cast<VkDeviceSize>(space);
    return alignedAddress;
}
#endif // VK_KHR_acceleration_structure
} // namespace magma
