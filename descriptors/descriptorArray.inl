namespace magma
{
namespace descriptor
{
inline DescriptorArray::DescriptorArray(VkDescriptorType descriptorType, uint32_t descriptorCount, uint32_t binding) noexcept:
    Descriptor(descriptorType, descriptorCount, binding)
{}

template<uint32_t Size>
inline void TDescriptorArray<Size>::getWriteDescriptor(VkDescriptorSet dstSet,
    VkWriteDescriptorSet& writeDescriptorSet) const noexcept
{
    writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSet.pNext = nullptr;
    writeDescriptorSet.dstSet = dstSet;
    writeDescriptorSet.dstBinding = binding.binding;
    writeDescriptorSet.dstArrayElement = 0;
    writeDescriptorSet.descriptorCount = binding.descriptorCount;
    writeDescriptorSet.descriptorType = binding.descriptorType;
    switch (binding.descriptorType)
    {
    case VK_DESCRIPTOR_TYPE_SAMPLER:
    case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
    case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
    case VK_DESCRIPTOR_TYPE_STORAGE_IMAGE:
    case VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT:
        writeDescriptorSet.pImageInfo = imageDescriptors;
        writeDescriptorSet.pBufferInfo = nullptr;
        writeDescriptorSet.pTexelBufferView = nullptr;
        break;
    case VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER:
    case VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER:
        writeDescriptorSet.pImageInfo = nullptr;
        writeDescriptorSet.pBufferInfo = nullptr;
        writeDescriptorSet.pTexelBufferView = texelBufferViews;
        break;
    case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
    case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER:
    case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC:
    case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC:
        writeDescriptorSet.pImageInfo = nullptr;
        writeDescriptorSet.pBufferInfo = bufferDescriptors;
        writeDescriptorSet.pTexelBufferView = nullptr;
        break;
    default:
        writeDescriptorSet.pImageInfo = nullptr;
        writeDescriptorSet.pBufferInfo = nullptr;
        writeDescriptorSet.pTexelBufferView = nullptr;
        MAGMA_ASSERT(false);
    }
    updated = false;
}

template<uint32_t Size>
inline DescriptorArray::ImageDescriptor TDescriptorArray<Size>::getImageElement(uint32_t index, VkImageUsageFlags requiredUsage) noexcept
{
    MAGMA_ASSERT(index < Size);
    return ImageDescriptor(imageDescriptors[index], requiredUsage, updated);
}

template<uint32_t Size>
inline DescriptorArray::BufferDescriptor TDescriptorArray<Size>::getBufferElement(uint32_t index, VkBufferUsageFlags requiredUsage) noexcept
{
    MAGMA_ASSERT(index < Size);
    return BufferDescriptor(bufferDescriptors[index], requiredUsage, updated);
}

template<uint32_t Size>
inline DescriptorArray::ImageDescriptor SamplerArray<Size>::operator[](uint32_t index) noexcept
{
    return TDescriptorArray<Size>::getImageElement(index, 0);
}

template<uint32_t Size>
inline DescriptorArray::ImmutableSamplerDescriptor ImmutableSamplerArray<Size>::operator[](uint32_t index) noexcept
{
    MAGMA_ASSERT(index < Size);
    return ImmutableSamplerDescriptor(immutableSamplers[index], updated);
}

template<uint32_t Size>
inline DescriptorArray::ImageDescriptor CombinedImageSamplerArray<Size>::operator[](uint32_t index) noexcept
{
    return TDescriptorArray<Size>::getImageElement(index, VK_IMAGE_USAGE_SAMPLED_BIT);
}

template<uint32_t Size>
inline DescriptorArray::ImageImmutableSamplerDescriptor CombinedImageImmutableSamplerArray<Size>::operator[](uint32_t index) noexcept
{
    MAGMA_ASSERT(index < Size);
    return ImageImmutableSamplerDescriptor(imageDescriptors[index], immutableSamplers[index], updated);
}

template<uint32_t Size>
inline DescriptorArray::ImageDescriptor SampledImageArray<Size>::operator[](uint32_t index) noexcept
{
    return TDescriptorArray<Size>::getImageElement(index, VK_IMAGE_USAGE_SAMPLED_BIT);
}

template<uint32_t Size>
inline DescriptorArray::ImageDescriptor StorageImageArray<Size>::operator[](uint32_t index) noexcept
{
    return TDescriptorArray<Size>::getImageElement(index, VK_IMAGE_USAGE_STORAGE_BIT);
}

template<uint32_t Size>
inline DescriptorArray::BufferDescriptor UniformTexelBufferArray<Size>::operator[](uint32_t index) noexcept
{
    return TDescriptorArray<Size>::getBufferElement(index, VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT);
}

template<uint32_t Size>
inline DescriptorArray::BufferDescriptor StorageTexelBufferArray<Size>::operator[](uint32_t index) noexcept
{
    return TDescriptorArray<Size>::getBufferElement(index, VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT);
}

template<uint32_t Size>
inline DescriptorArray::BufferDescriptor UniformBufferArray<Size>::operator[](uint32_t index) noexcept
{
    return TDescriptorArray<Size>::getBufferElement(index, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
}

template<uint32_t Size>
inline DescriptorArray::BufferDescriptor StorageBufferArray<Size>::operator[](uint32_t index) noexcept
{
    return TDescriptorArray<Size>::getBufferElement(index, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
}

template<uint32_t Size>
inline DescriptorArray::BufferDescriptor DynamicUniformBufferArray<Size>::operator[](uint32_t index) noexcept
{
    return TDescriptorArray<Size>::getBufferElement(index, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
}

template<uint32_t Size>
inline DescriptorArray::BufferDescriptor DynamicStorageBufferArray<Size>::operator[](uint32_t index) noexcept
{
    return TDescriptorArray<Size>::getBufferElement(index, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
}

template<uint32_t Size>
inline DescriptorArray::ImageDescriptor InputAttachmentArray<Size>::operator[](uint32_t index) noexcept
{
    return TDescriptorArray<Size>::getImageElement(index, VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT);
}
} // namespace descriptor
} // namespace magma
