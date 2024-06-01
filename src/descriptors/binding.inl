namespace magma
{
namespace descriptor
{
inline Binding::Binding(VkDescriptorType descriptorType, uint32_t descriptorCount, uint32_t binding_) noexcept:
    binding{
        binding_,
        descriptorType,
        descriptorCount,
        0, // stageFlags
        nullptr // pImmutableSamples
    },
    imageType(VK_IMAGE_TYPE_MAX_ENUM),
    dirty(false)
{}

inline void Binding::setImageType(VkImageType imageType_) noexcept
{
    if (imageType != VK_IMAGE_TYPE_MAX_ENUM)
    {   // Descriptor should have the same image type
        MAGMA_ASSERT(imageType_ == imageType);
    }
    imageType = imageType_;
}
} // namespace descriptor
} // namespace magma
