namespace magma
{
namespace descriptor
{
inline void ImageDescriptor::write(VkDescriptorSet dstSet, VkWriteDescriptorSet& writeDescriptorSet) const noexcept
{
#ifdef MAGMA_DEBUG
    switch (binding.descriptorType)
    {
    case VK_DESCRIPTOR_TYPE_SAMPLER:
        MAGMA_ASSERT(descriptor.sampler);
        break;
    case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
        MAGMA_ASSERT(descriptor.imageView != VK_NULL_HANDLE);
        MAGMA_ASSERT(descriptor.sampler || binding.pImmutableSamplers);
        break;
    case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
    case VK_DESCRIPTOR_TYPE_STORAGE_IMAGE:
    case VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT:
        MAGMA_ASSERT(descriptor.imageView != VK_NULL_HANDLE);
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
    writeDescriptorSet.pImageInfo = &descriptor;
    writeDescriptorSet.pBufferInfo = nullptr;
    writeDescriptorSet.pTexelBufferView = nullptr;
    updated = false;
}

inline void ImageDescriptor::updateImageView(std::shared_ptr<const ImageView> imageView, VkImageUsageFlags requiredUsage) noexcept
{
    MAGMA_UNUSED(requiredUsage);
    MAGMA_ASSERT(imageView);
    MAGMA_ASSERT(imageView->getImage()->getUsage() & requiredUsage);
    if (descriptor.imageView != *imageView)
    {
        descriptor = imageView->getDescriptor(nullptr);
        imageType = imageView->getImage()->getType();
        updated = true;
    }
}

inline Sampler& Sampler::operator=(std::shared_ptr<const magma::Sampler> sampler) noexcept
{
    MAGMA_ASSERT(sampler);
    descriptor.sampler = *sampler;
    descriptor.imageView = VK_NULL_HANDLE;
    descriptor.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    updated = true;
    return *this;
}

inline CombinedImageSampler& CombinedImageSampler::operator=(const std::pair<std::shared_ptr<const ImageView>, std::shared_ptr<const magma::Sampler>>& combinedImageSampler) noexcept
{
    MAGMA_ASSERT(combinedImageSampler.first);
    MAGMA_ASSERT(combinedImageSampler.second);
    //MAGMA_ASSERT(combinedImageSampler.first->getImage()->getUsage() & VK_IMAGE_USAGE_SAMPLED_BIT);
    if ((descriptor.imageView != *combinedImageSampler.first) ||
        (descriptor.sampler != *combinedImageSampler.second))
    {
        descriptor = combinedImageSampler.first->getDescriptor(combinedImageSampler.second);
        updated = true;
    }
    return *this;
}

inline CombinedImageImmutableSampler& CombinedImageImmutableSampler::operator=(const std::pair<std::shared_ptr<const ImageView>, std::shared_ptr<const magma::Sampler>>& combinedImageSampler) noexcept
{
    updateImageView(combinedImageSampler.first, VK_IMAGE_USAGE_SAMPLED_BIT);
    // Immutable sampler must be updated only once
    if (!binding.pImmutableSamplers)
    {   // If pImmutableSamplers is not NULL, then it is a pointer to an array of sampler handles
        // that will be copied into the set layout and used for the corresponding binding.
        MAGMA_ASSERT(combinedImageSampler.second);
        binding.pImmutableSamplers = combinedImageSampler.second->getHandleAddress();
        updated = true;
    }
    return *this;
}

inline CombinedImageImmutableSampler& CombinedImageImmutableSampler::operator=(std::shared_ptr<const ImageView> imageView) noexcept
{   // Check that sampler is already set and stop carrying around it
    MAGMA_ASSERT(binding.pImmutableSamplers);
    updateImageView(std::move(imageView), VK_IMAGE_USAGE_SAMPLED_BIT);
    return *this;
}

inline SampledImage& SampledImage::operator=(std::shared_ptr<const ImageView> imageView) noexcept
{
    updateImageView(std::move(imageView), VK_IMAGE_USAGE_SAMPLED_BIT);
    return *this;
}

inline StorageImage& StorageImage::operator=(std::shared_ptr<const ImageView> imageView) noexcept
{
    updateImageView(std::move(imageView), VK_IMAGE_USAGE_STORAGE_BIT);
    return *this;
}

inline InputAttachment& InputAttachment::operator=(std::shared_ptr<const ImageView> imageView) noexcept
{
    updateImageView(std::move(imageView), VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT);
    return *this;
}
} // namespace descriptor
} // namespace magma
