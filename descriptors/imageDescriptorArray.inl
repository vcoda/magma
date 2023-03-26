namespace magma
{
namespace descriptor
{
template<uint32_t Size>
inline ImageDescriptorArray<Size>::ImageDescriptorArray(VkDescriptorType descriptorType, uint32_t binding) noexcept:
    DescriptorArray<VkDescriptorImageInfo, Size>(descriptorType, binding)
{}

template<uint32_t Size>
inline void ImageDescriptorArray<Size>::write(VkDescriptorSet dstSet, VkWriteDescriptorSet& writeDescriptorSet) const noexcept
{
#ifdef MAGMA_DEBUG
    switch (binding.descriptorType)
    {
    case VK_DESCRIPTOR_TYPE_SAMPLER:
        MAGMA_ASSERT(std::any_of(descriptors.begin(), descriptors.end(),
            [](const auto& it)
            {
                return it.sampler != VK_NULL_HANDLE;
            }));
        break;
    case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
    case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
    case VK_DESCRIPTOR_TYPE_STORAGE_IMAGE:
    case VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT:
        MAGMA_ASSERT(std::any_of(descriptors.begin(), descriptors.end(),
            [](const auto& it)
            {
                return it.imageView != VK_NULL_HANDLE;
            }));
        break;
    }
#endif // MAGMA_DEBUG
    writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSet.pNext = nullptr;
    writeDescriptorSet.dstSet = dstSet;
    writeDescriptorSet.dstBinding = binding.binding;
    writeDescriptorSet.dstArrayElement = 0;
    writeDescriptorSet.descriptorCount = binding.descriptorCount;
    writeDescriptorSet.descriptorType = binding.descriptorType;
    writeDescriptorSet.pImageInfo = descriptors.data();
    writeDescriptorSet.pBufferInfo = nullptr;
    writeDescriptorSet.pTexelBufferView = nullptr;
    updated = false;
}

template<uint32_t Size>
inline array::ImageDescriptor ImageDescriptorArray<Size>::getArrayElement(uint32_t index, VkImageUsageFlags requiredUsage) noexcept
{
    return array::ImageDescriptor(descriptors[index], imageType, requiredUsage, updated);
}

template<uint32_t Size>
inline array::SamplerDescriptor SamplerArray<Size>::operator[](uint32_t index) noexcept
{
    return array::SamplerDescriptor(descriptors[index], updated);
}

template<uint32_t Size>
inline array::ImageSamplerDescriptor CombinedImageSamplerArray<Size>::operator[](uint32_t index) noexcept
{
    return array::ImageSamplerDescriptor(descriptors[index], imageType, VK_IMAGE_USAGE_SAMPLED_BIT, updated);
}

template<uint32_t Size>
inline array::ImageImmutableSamplerDescriptor CombinedImageImmutableSamplerArray<Size>::operator[](uint32_t index) noexcept
{
    return array::ImageImmutableSamplerDescriptor(descriptors[index], immutableSamplers[index], imageType, VK_IMAGE_USAGE_SAMPLED_BIT, updated);
}

template<uint32_t Size>
inline array::ImageDescriptor SampledImageArray<Size>::operator[](uint32_t index) noexcept
{
    return this->getArrayElement(index, VK_IMAGE_USAGE_SAMPLED_BIT);
}

template<uint32_t Size>
inline array::ImageDescriptor StorageImageArray<Size>::operator[](uint32_t index) noexcept
{
    return this->getArrayElement(index, VK_IMAGE_USAGE_STORAGE_BIT);
}

template<uint32_t Size>
inline array::ImageDescriptor InputAttachmentArray<Size>::operator[](uint32_t index) noexcept
{
    return this->getArrayElement(index, VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT);
}
} // namespace descriptor
} // namespace magma
