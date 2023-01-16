namespace magma
{
namespace binding
{
template<uint32_t Size>
inline void SamplerArray<Size>::Descriptor::operator=(std::shared_ptr<const magma::Sampler> sampler) noexcept
{
    MAGMA_ASSERT(sampler);
    imageDescriptor.sampler = *sampler;
    imageDescriptor.imageView = VK_NULL_HANDLE;
    imageDescriptor.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
}

template<uint32_t Size>
inline typename SamplerArray<Size>::Descriptor SamplerArray<Size>::operator[](uint32_t index) noexcept
{
    MAGMA_ASSERT(index < Size);
    updated = true;
    return Descriptor(imageDescriptors[index]);
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
    MAGMA_ASSERT(VK_NULL_HANDLE == immutableSampler);
    imageDescriptor = combinedImageSampler.first->getDescriptor(nullptr);
    if (VK_NULL_HANDLE == immutableSampler) // Immutable sampler must be updated only once
        immutableSampler = *combinedImageSampler.second;
}

template<uint32_t Size>
inline void CombinedImageImmutableSamplerArray<Size>::Descriptor::operator=(std::shared_ptr<const ImageView> imageView) noexcept
{
    MAGMA_ASSERT(imageView);
    MAGMA_ASSERT(immutableSampler != VK_NULL_HANDLE); // Check that sampler is already set and stop carrying around it
    imageDescriptor = imageView->getDescriptor(nullptr);
    return *this;
}

template<uint32_t Size>
inline typename CombinedImageImmutableSamplerArray<Size>::Descriptor CombinedImageImmutableSamplerArray<Size>::operator[](uint32_t index) noexcept
{
    MAGMA_ASSERT(index < Size);
    updated = true;
    return Descriptor(imageDescriptors[index], immutableSamplers[index]);
}

template<uint32_t Size>
inline void StorageBufferArray<Size>::Descriptor::operator=(std::shared_ptr<const Buffer> buffer) noexcept
{
    MAGMA_ASSERT(buffer);
    MAGMA_ASSERT(buffer->getUsage() & VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
    bufferDescriptor = buffer->getDescriptor();
}

template<uint32_t Size>
inline typename StorageBufferArray<Size>::Descriptor StorageBufferArray<Size>::operator[](uint32_t index) noexcept
{
    MAGMA_ASSERT(index < Size);
    updated = true;
    return Descriptor(bufferDescriptors[index]);
}
} // namespace binding
} // namespace magma
