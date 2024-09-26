namespace magma
{
template<class Pointer>
inline TImageView<Pointer>::TImageView(Pointer image,
    VkImageViewCreateFlags flags /* 0 */,
    VkImageUsageFlags usage /* 0 */,
    const StructureChain& extendedInfo /* default */):
    TImageView(std::move(image), 0, VK_REMAINING_MIP_LEVELS, 0, VK_REMAINING_ARRAY_LAYERS,
        {VK_COMPONENT_SWIZZLE_IDENTITY,
         VK_COMPONENT_SWIZZLE_IDENTITY,
         VK_COMPONENT_SWIZZLE_IDENTITY,
         VK_COMPONENT_SWIZZLE_IDENTITY},
        flags, usage, extendedInfo)
{}

template<class Pointer>
inline TImageView<Pointer>::TImageView(Pointer image, const VkComponentMapping& swizzle,
    VkImageViewCreateFlags flags /* 0 */,
    VkImageUsageFlags usage /* 0 */,
    const StructureChain& extendedInfo /* default */):
    TImageView(std::move(image), 0, VK_REMAINING_MIP_LEVELS, 0, VK_REMAINING_ARRAY_LAYERS,
        swizzle, flags, usage, extendedInfo)
{}

template<class Pointer>
inline TImageView<Pointer>::TImageView(Pointer image, uint32_t baseMipLevel,
    uint32_t levelCount /* VK_REMAINING_MIP_LEVELS */,
    uint32_t baseArrayLayer /* 0 */,
    uint32_t layerCount /* VK_REMAINING_ARRAY_LAYERS */,
    const VkComponentMapping& swizzle /* VK_COMPONENT_SWIZZLE_IDENTITY */,
    VkImageViewCreateFlags flags /* 0 */,
    VkImageUsageFlags usage /* 0 */,
    const StructureChain& extendedInfo /* default */):
    ImageView(image.get(), baseMipLevel, levelCount, baseArrayLayer, layerCount, swizzle, flags, usage, extendedInfo),
    image(std::move(image))
{}
} // namespace magma
