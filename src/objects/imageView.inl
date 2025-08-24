namespace magma
{
template<class Pointer>
inline TImageView<Pointer>::TImageView(Pointer image,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkImageViewCreateFlags flags /* 0 */,
    VkImageUsageFlags usage /* 0 */,
    const StructureChain& extendedInfo /* default */):
    TImageView(std::move(image), 0, VK_REMAINING_MIP_LEVELS, 0, VK_REMAINING_ARRAY_LAYERS,
        std::move(allocator),
        {VK_COMPONENT_SWIZZLE_IDENTITY,
         VK_COMPONENT_SWIZZLE_IDENTITY,
         VK_COMPONENT_SWIZZLE_IDENTITY,
         VK_COMPONENT_SWIZZLE_IDENTITY},
        flags, usage, extendedInfo)
{}

template<class Pointer>
inline TImageView<Pointer>::TImageView(Pointer image, const VkComponentMapping& swizzling,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkImageViewCreateFlags flags /* 0 */,
    VkImageUsageFlags usage /* 0 */,
    const StructureChain& extendedInfo /* default */):
    TImageView(std::move(image), 0, VK_REMAINING_MIP_LEVELS, 0, VK_REMAINING_ARRAY_LAYERS,
        std::move(allocator), swizzling, flags, usage, extendedInfo)
{}

template<class Pointer>
inline TImageView<Pointer>::TImageView(Pointer image, uint32_t baseMipLevel,
    uint32_t levelCount /* VK_REMAINING_MIP_LEVELS */,
    uint32_t baseArrayLayer /* 0 */,
    uint32_t layerCount /* VK_REMAINING_ARRAY_LAYERS */,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    const VkComponentMapping& swizzling /* VK_COMPONENT_SWIZZLE_IDENTITY */,
    VkImageViewCreateFlags flags /* 0 */,
    VkImageUsageFlags usage /* 0 */,
    const StructureChain& extendedInfo /* default */):
    ImageView(image.get(), baseMipLevel, levelCount, baseArrayLayer, layerCount, swizzling, flags, usage, std::move(allocator), extendedInfo),
    image(std::move(image))
{}
} // namespace magma
