namespace magma
{
namespace descriptor
{
inline Binding::Binding(VkDescriptorType descriptorType, uint32_t descriptorCount, uint32_t binding_) noexcept
{
    binding.binding = binding_;
    binding.descriptorType = descriptorType;
    binding.descriptorCount = descriptorCount;
    binding.stageFlags = 0;
    binding.pImmutableSamplers = nullptr;
}

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
