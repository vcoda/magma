namespace magma
{
namespace descriptor
{
template<uint32_t Size>
inline ImageDescriptorArray<Size>::ImageDescriptorArray(VkDescriptorType descriptorType, uint32_t binding) noexcept:
    DescriptorArray<VkDescriptorImageInfo, Size>(descriptorType, binding)
{}

template<uint32_t Size>
inline bool ImageDescriptorArray<Size>::associatedWithResource() const noexcept
{
    switch (Super::binding.descriptorType)
    {
    case VK_DESCRIPTOR_TYPE_SAMPLER:
        return std::all_of(Super::descriptors.begin(), Super::descriptors.end(),
            [](const auto& it)
            {
                return (it.sampler != VK_NULL_HANDLE);
            });
    case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
    case VK_DESCRIPTOR_TYPE_STORAGE_IMAGE:
    case VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT:
        return std::all_of(Super::descriptors.begin(), Super::descriptors.end(),
            [](const auto& it)
            {
                return (it.imageView != VK_NULL_HANDLE);
            });
    default:
        return false;
    }
}

template<uint32_t Size>
inline void ImageDescriptorArray<Size>::write(VkDescriptorSet dstSet, VkWriteDescriptorSet& writeDescriptorSet) const noexcept
{
    MAGMA_ASSERT(associatedWithResource());
    writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSet.pNext = nullptr;
    writeDescriptorSet.dstSet = dstSet;
    writeDescriptorSet.dstBinding = Super::binding.binding;
    writeDescriptorSet.dstArrayElement = 0;
    writeDescriptorSet.descriptorCount = Super::binding.descriptorCount;
    writeDescriptorSet.descriptorType = Super::binding.descriptorType;
    writeDescriptorSet.pImageInfo = Super::descriptors.data();
    writeDescriptorSet.pBufferInfo = nullptr;
    writeDescriptorSet.pTexelBufferView = nullptr;
    Super::updated = false;
}

template<uint32_t Size>
inline array::ImageDescriptor ImageDescriptorArray<Size>::getArrayElement(uint32_t index, VkImageUsageFlags requiredUsage) noexcept
{
    return array::ImageDescriptor(Super::descriptors[index], Super::imageType, requiredUsage, Super::updated);
}

template<uint32_t Size>
inline array::SamplerDescriptor SamplerArray<Size>::operator[](uint32_t index) noexcept
{
    return array::SamplerDescriptor(DescriptorArray<VkDescriptorImageInfo, Size>::descriptors[index], Binding::updated);
}

template<uint32_t Size>
inline array::ImageSamplerDescriptor CombinedImageSamplerArray<Size>::operator[](uint32_t index) noexcept
{
    return array::ImageSamplerDescriptor(DescriptorArray<VkDescriptorImageInfo, Size>::descriptors[index], Binding::imageType, VK_IMAGE_USAGE_SAMPLED_BIT, Binding::updated);
}

template<uint32_t Size>
inline bool CombinedImageImmutableSamplerArray<Size>::associatedWithResource() const noexcept
{
    const bool associatedWithSamplers = std::all_of(Super::descriptors.begin(), Super::descriptors.end(),
        [](const auto& it)
        {
            return (it.sampler != VK_NULL_HANDLE);
        }) || std::all_of(immutableSamplers.begin(), immutableSamplers.end(),
        [](VkSampler it)
        {
            return (it != VK_NULL_HANDLE);
        });
    return std::all_of(Super::descriptors.begin(), Super::descriptors.end(),
        [](const auto& it)
        {
            return (it.imageView != VK_NULL_HANDLE);
        }) && associatedWithSamplers;
}

template<uint32_t Size>
inline array::ImageImmutableSamplerDescriptor CombinedImageImmutableSamplerArray<Size>::operator[](uint32_t index) noexcept
{
    return array::ImageImmutableSamplerDescriptor(Super::descriptors[index], immutableSamplers[index], Binding::imageType, VK_IMAGE_USAGE_SAMPLED_BIT, Binding::updated);
}

template<uint32_t Size>
inline array::ImageDescriptor SampledImageArray<Size>::operator[](uint32_t index) noexcept
{
    return ImageDescriptorArray<Size>::getArrayElement(index, VK_IMAGE_USAGE_SAMPLED_BIT);
}

template<uint32_t Size>
inline array::ImageDescriptor StorageImageArray<Size>::operator[](uint32_t index) noexcept
{
    return ImageDescriptorArray<Size>::getArrayElement(index, VK_IMAGE_USAGE_STORAGE_BIT);
}

template<uint32_t Size>
inline array::ImageDescriptor InputAttachmentArray<Size>::operator[](uint32_t index) noexcept
{
    return ImageDescriptorArray<Size>::getArrayElement(index, VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT);
}
} // namespace descriptor
} // namespace magma
