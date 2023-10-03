namespace magma
{
namespace descriptor
{
inline ImageDescriptor::ImageDescriptor(VkDescriptorType descriptorType, uint32_t binding) noexcept:
    Descriptor(descriptorType, binding)
{}

inline bool ImageDescriptor::associatedWithResource() const noexcept
{
    switch (binding.descriptorType)
    {
    case VK_DESCRIPTOR_TYPE_SAMPLER:
        return (descriptor.sampler != VK_NULL_HANDLE);
    case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
    case VK_DESCRIPTOR_TYPE_STORAGE_IMAGE:
    case VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT:
        return (descriptor.imageView != VK_NULL_HANDLE);
    case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
        return (descriptor.imageView != VK_NULL_HANDLE) && ((descriptor.sampler != VK_NULL_HANDLE) || (binding.pImmutableSamplers != nullptr));
    default:
        return false;
    }
}

inline void ImageDescriptor::write(VkDescriptorSet dstSet, VkWriteDescriptorSet& writeDescriptorSet) const noexcept
{
    MAGMA_ASSERT(associatedWithResource());
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

inline void ImageDescriptor::updateImageView(std::shared_ptr<const ImageView> imageView, std::shared_ptr<const magma::Sampler> sampler,
    VkImageUsageFlags requiredUsage) noexcept
{
    MAGMA_UNUSED(requiredUsage);
    MAGMA_ASSERT(imageView);
    const std::shared_ptr<Image>& image = imageView->getImage();
    MAGMA_ASSERT(image->getUsage() & requiredUsage);
    if ((descriptor.imageView != *imageView) ||
        (sampler && descriptor.sampler != *sampler))
    {
        setImageType(image->getType());
        descriptor = imageView->getDescriptor(sampler); // May be null
        if (VK_IMAGE_LAYOUT_UNDEFINED == descriptor.imageLayout)
        {   // Assume that later image layout will be changed,
            // e.g. when a render pass instance ends.
            const Format format(image->getFormat());
            if (format.depth() || format.stencil() || format.depthStencil())
                descriptor.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
            else
                descriptor.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        }
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

inline CombinedImageSampler& CombinedImageSampler::operator=(const ImageSamplerPair& combinedImageSampler) noexcept
{
    std::shared_ptr<const ImageView> imageView = combinedImageSampler.first;
    MAGMA_ASSERT(imageView);
    MAGMA_ASSERT(combinedImageSampler.second);
    updateImageView(combinedImageSampler.first, combinedImageSampler.second, VK_IMAGE_USAGE_SAMPLED_BIT);
    return *this;
}

inline CombinedImageImmutableSampler& CombinedImageImmutableSampler::operator=(const ImageSamplerPair& combinedImageSampler) noexcept
{
    MAGMA_ASSERT(!binding.pImmutableSamplers);
    updateImageView(combinedImageSampler.first, nullptr, VK_IMAGE_USAGE_SAMPLED_BIT);
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
    updateImageView(std::move(imageView), nullptr, VK_IMAGE_USAGE_SAMPLED_BIT);
    return *this;
}

inline SampledImage& SampledImage::operator=(std::shared_ptr<const ImageView> imageView) noexcept
{
    updateImageView(std::move(imageView), nullptr, VK_IMAGE_USAGE_SAMPLED_BIT);
    return *this;
}

inline StorageImage& StorageImage::operator=(std::shared_ptr<const ImageView> imageView) noexcept
{
    updateImageView(std::move(imageView), nullptr, VK_IMAGE_USAGE_STORAGE_BIT);
    return *this;
}

inline InputAttachment& InputAttachment::operator=(std::shared_ptr<const ImageView> imageView) noexcept
{
    updateImageView(std::move(imageView), nullptr, VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT);
    return *this;
}
} // namespace descriptor
} // namespace magma
