namespace magma
{
namespace descriptor
{
template<uint32_t Size>
inline void ImageDescriptorArray<Size>::write(VkDescriptorSet dstSet, VkWriteDescriptorSet& writeDescriptorSet) const noexcept
{
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
inline DescriptorArray::ImageDescriptor ImageDescriptorArray<Size>::getArrayElement(uint32_t index, VkImageUsageFlags requiredUsage) noexcept
{
    return ImageDescriptor(descriptors[index], imageType, requiredUsage, updated);
}

template<uint32_t Size>
inline DescriptorArray::ImageDescriptor SamplerArray<Size>::operator[](uint32_t index) noexcept
{
    return this->getArrayElement(index, 0);
}

template<uint32_t Size>
inline DescriptorArray::ImmutableSamplerDescriptor ImmutableSamplerArray<Size>::operator[](uint32_t index) noexcept
{
    return ImmutableSamplerDescriptor(immutableSamplers[index], updated);
}

template<uint32_t Size>
inline DescriptorArray::ImageDescriptor CombinedImageSamplerArray<Size>::operator[](uint32_t index) noexcept
{
    return this->getArrayElement(index, VK_IMAGE_USAGE_SAMPLED_BIT);
}

template<uint32_t Size>
inline DescriptorArray::ImageImmutableSamplerDescriptor CombinedImageImmutableSamplerArray<Size>::operator[](uint32_t index) noexcept
{
    return ImageImmutableSamplerDescriptor(descriptors[index], immutableSamplers[index], imageType, updated);
}

template<uint32_t Size>
inline DescriptorArray::ImageDescriptor SampledImageArray<Size>::operator[](uint32_t index) noexcept
{
    return this->getArrayElement(index, VK_IMAGE_USAGE_SAMPLED_BIT);
}

template<uint32_t Size>
inline DescriptorArray::ImageDescriptor StorageImageArray<Size>::operator[](uint32_t index) noexcept
{
    return this->getArrayElement(index, VK_IMAGE_USAGE_STORAGE_BIT);
}

template<uint32_t Size>
inline DescriptorArray::ImageDescriptor InputAttachmentArray<Size>::operator[](uint32_t index) noexcept
{
    return this->getArrayElement(index, VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT);
}
} // namespace descriptor
} // namespace magma
