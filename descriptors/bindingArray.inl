namespace magma
{
namespace binding
{
inline void SamplerArray::DescriptorImageInfo::operator=(std::shared_ptr<const magma::Sampler> sampler_) noexcept
{
    sampler = *sampler_;
    imageView = VK_NULL_HANDLE;
    imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
}

inline void CombinedImageSamplerArray::DescriptorImageInfo::operator=(const std::pair<std::shared_ptr<const ImageView>, std::shared_ptr<const magma::Sampler>>& combinedImageSampler) noexcept
{
    const VkDescriptorImageInfo imageDescriptor = combinedImageSampler.first->getDescriptor(combinedImageSampler.second);
    *(VkDescriptorImageInfo *)this = imageDescriptor;
}

inline void StorageBufferArray::DescriptorBufferInfo::operator=(std::shared_ptr<const magma::Buffer> buffer) noexcept
{
    MAGMA_ASSERT(buffer->getUsage() & VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
    const VkDescriptorBufferInfo bufferDescriptor = buffer->getDescriptor();
    *(VkDescriptorBufferInfo *)this = bufferDescriptor;
}
} // namespace binding
} // namespace magma
