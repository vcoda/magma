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
