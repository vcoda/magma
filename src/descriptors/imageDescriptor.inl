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
        return (descriptor.imageView != VK_NULL_HANDLE) &&
            ((descriptor.sampler != VK_NULL_HANDLE) || (binding.pImmutableSamplers != nullptr));
    default:
        return false;
    }
}

inline Sampler& Sampler::operator=(std::shared_ptr<const magma::Sampler> sampler) noexcept
{
    MAGMA_ASSERT(sampler);
    descriptor.sampler = *sampler;
    descriptor.imageView = VK_NULL_HANDLE;
    descriptor.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    dirty = true;
    return *this;
}

inline CombinedImageSampler& CombinedImageSampler::operator=(const ImageSampler& imageSampler) noexcept
{
    std::shared_ptr<const ImageView> imageView = imageSampler.first;
    MAGMA_ASSERT(imageView);
    MAGMA_ASSERT(imageSampler.second);
    update(imageSampler.first, imageSampler.second, VK_IMAGE_USAGE_SAMPLED_BIT);
    return *this;
}

inline CombinedImageImmutableSampler& CombinedImageImmutableSampler::operator=(const ImageSampler& imageSampler) noexcept
{
    MAGMA_ASSERT(!binding.pImmutableSamplers);
    update(imageSampler.first, nullptr, VK_IMAGE_USAGE_SAMPLED_BIT);
    // Immutable sampler must be updated only once
    if (!binding.pImmutableSamplers)
    {   // If pImmutableSamplers is not NULL, then it is a pointer to an array of sampler handles
        // that will be copied into the set layout and used for the corresponding binding.
        MAGMA_ASSERT(imageSampler.second);
        binding.pImmutableSamplers = imageSampler.second->getHandleAddress();
        dirty = true;
    }
    return *this;
}

inline CombinedImageImmutableSampler& CombinedImageImmutableSampler::operator=(std::shared_ptr<const ImageView> imageView) noexcept
{   // Check that sampler is already set and stop carrying around it
    MAGMA_ASSERT(binding.pImmutableSamplers);
    update(std::move(imageView), nullptr, VK_IMAGE_USAGE_SAMPLED_BIT);
    return *this;
}

inline SampledImage& SampledImage::operator=(std::shared_ptr<const ImageView> imageView) noexcept
{
    update(std::move(imageView), nullptr, VK_IMAGE_USAGE_SAMPLED_BIT);
    return *this;
}

inline StorageImage& StorageImage::operator=(std::shared_ptr<const ImageView> imageView) noexcept
{
    update(std::move(imageView), nullptr, VK_IMAGE_USAGE_STORAGE_BIT);
    return *this;
}

inline InputAttachment& InputAttachment::operator=(std::shared_ptr<const ImageView> imageView) noexcept
{
    update(std::move(imageView), nullptr, VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT);
    return *this;
}
} // namespace descriptor
} // namespace magma
