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
inline DescriptorArray::ImageDescriptor SamplerArray<Size>::operator[](uint32_t index) noexcept
{
    MAGMA_ASSERT(index < Size);
    return DescriptorArray::ImageDescriptor(0, TDescriptorArray<Size>::imageDescriptors[index], Descriptor::updated);
}

template<uint32_t Size>
inline void ImmutableSamplerArray<Size>::Descriptor::operator=(std::shared_ptr<const magma::Sampler> sampler) noexcept
{
    MAGMA_ASSERT(sampler);
    MAGMA_ASSERT(VK_NULL_HANDLE == immutableSampler);
    imageDescriptor.sampler = VK_NULL_HANDLE;
    imageDescriptor.imageView = VK_NULL_HANDLE;
    imageDescriptor.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    if (VK_NULL_HANDLE == immutableSampler) // Immutable sampler must be updated only once
        immutableSampler = *sampler;
}

template<uint32_t Size>
inline typename ImmutableSamplerArray<Size>::Descriptor ImmutableSamplerArray<Size>::operator[](uint32_t index) noexcept
{
    MAGMA_ASSERT(index < Size);
    MAGMA_ASSERT(VK_NULL_HANDLE == immutableSamplers[index]);
    updated = true;
    return Descriptor(imageDescriptors[index], immutableSamplers[index]);
}

template<uint32_t Size>
inline void CombinedImageSamplerArray<Size>::Descriptor::operator=(const std::pair<std::shared_ptr<const ImageView>, std::shared_ptr<const magma::Sampler>>& combinedImageSampler) noexcept
{
    MAGMA_ASSERT(combinedImageSampler.first);
    MAGMA_ASSERT(combinedImageSampler.second);
    MAGMA_ASSERT(combinedImageSampler.first->getImage()->getUsage() & VK_IMAGE_USAGE_SAMPLED_BIT);
    imageDescriptor = combinedImageSampler.first->getDescriptor(combinedImageSampler.second);
}

template<uint32_t Size>
inline typename CombinedImageSamplerArray<Size>::Descriptor CombinedImageSamplerArray<Size>::operator[](uint32_t index) noexcept
{
    MAGMA_ASSERT(index < Size);
    updated = true;
    return Descriptor(imageDescriptors[index]);
}

template<uint32_t Size>
inline void CombinedImageImmutableSamplerArray<Size>::Descriptor::operator=(const std::pair<std::shared_ptr<const ImageView>, std::shared_ptr<const magma::Sampler>>& combinedImageSampler) noexcept
{
    MAGMA_ASSERT(combinedImageSampler.first);
    MAGMA_ASSERT(combinedImageSampler.second);
    MAGMA_ASSERT(combinedImageSampler.first->getImage()->getUsage() & VK_IMAGE_USAGE_SAMPLED_BIT);
    MAGMA_ASSERT(VK_NULL_HANDLE == immutableSampler);
    imageDescriptor = combinedImageSampler.first->getDescriptor(nullptr);
    if (VK_NULL_HANDLE == immutableSampler) // Immutable sampler must be updated only once
        immutableSampler = *combinedImageSampler.second;
}

template<uint32_t Size>
inline void CombinedImageImmutableSamplerArray<Size>::Descriptor::operator=(std::shared_ptr<const ImageView> imageView) noexcept
{
    MAGMA_ASSERT(imageView);
    MAGMA_ASSERT(imageView->getImage()->getUsage() & VK_IMAGE_USAGE_SAMPLED_BIT);
    MAGMA_ASSERT(immutableSampler != VK_NULL_HANDLE); // Check that sampler is already set and stop carrying around it
    imageDescriptor = imageView->getDescriptor(nullptr);
}

template<uint32_t Size>
inline typename CombinedImageImmutableSamplerArray<Size>::Descriptor CombinedImageImmutableSamplerArray<Size>::operator[](uint32_t index) noexcept
{
    MAGMA_ASSERT(index < Size);
    updated = true;
    return Descriptor(imageDescriptors[index], immutableSamplers[index]);
}

template<uint32_t Size>
inline DescriptorArray::ImageDescriptor SampledImageArray<Size>::operator[](uint32_t index) noexcept
{
    MAGMA_ASSERT(index < Size);
    return DescriptorArray::ImageDescriptor(VK_IMAGE_USAGE_SAMPLED_BIT, TDescriptorArray<Size>::imageDescriptors[index], Descriptor::updated);
}

template<uint32_t Size>
inline DescriptorArray::ImageDescriptor StorageImageArray<Size>::operator[](uint32_t index) noexcept
{
    MAGMA_ASSERT(index < Size);
    return DescriptorArray::ImageDescriptor(VK_IMAGE_USAGE_STORAGE_BIT, TDescriptorArray<Size>::imageDescriptors[index], Descriptor::updated);
}

template<uint32_t Size>
inline DescriptorArray::BufferDescriptor UniformTexelBufferArray<Size>::operator[](uint32_t index) noexcept
{
    MAGMA_ASSERT(index < Size);
    return DescriptorArray::BufferDescriptor(VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT, TDescriptorArray<Size>::bufferDescriptors[index], Descriptor::updated);
}

template<uint32_t Size>
inline DescriptorArray::BufferDescriptor StorageTexelBufferArray<Size>::operator[](uint32_t index) noexcept
{
    MAGMA_ASSERT(index < Size);
    return DescriptorArray::BufferDescriptor(VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT, TDescriptorArray<Size>::bufferDescriptors[index], Descriptor::updated);
}

template<uint32_t Size>
inline DescriptorArray::BufferDescriptor UniformBufferArray<Size>::operator[](uint32_t index) noexcept
{
    MAGMA_ASSERT(index < Size);
    return DescriptorArray::BufferDescriptor(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, TDescriptorArray<Size>::bufferDescriptors[index], Descriptor::updated);
}

template<uint32_t Size>
inline DescriptorArray::BufferDescriptor StorageBufferArray<Size>::operator[](uint32_t index) noexcept
{
    MAGMA_ASSERT(index < Size);
    return DescriptorArray::BufferDescriptor(VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, TDescriptorArray<Size>::bufferDescriptors[index], Descriptor::updated);
}

template<uint32_t Size>
inline DescriptorArray::BufferDescriptor DynamicUniformBufferArray<Size>::operator[](uint32_t index) noexcept
{
    MAGMA_ASSERT(index < Size);
    return DescriptorArray::BufferDescriptor(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, TDescriptorArray<Size>::bufferDescriptors[index], Descriptor::updated);
}

template<uint32_t Size>
inline DescriptorArray::BufferDescriptor DynamicStorageBufferArray<Size>::operator[](uint32_t index) noexcept
{
    MAGMA_ASSERT(index < Size);
    return DescriptorArray::BufferDescriptor(VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, TDescriptorArray<Size>::bufferDescriptors[index], Descriptor::updated);
}

template<uint32_t Size>
inline DescriptorArray::ImageDescriptor InputAttachmentArray<Size>::operator[](uint32_t index) noexcept
{
    MAGMA_ASSERT(index < Size);
    return DescriptorArray::ImageDescriptor(VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT, TDescriptorArray<Size>::imageDescriptors[index], Descriptor::updated);
}
} // namespace descriptor
} // namespace magma
