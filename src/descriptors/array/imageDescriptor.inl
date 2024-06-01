namespace magma
{
namespace descriptor
{
namespace array
{
inline SamplerDescriptor::SamplerDescriptor(VkDescriptorImageInfo& descriptor, bool& updated) noexcept:
    Descriptor<VkDescriptorImageInfo>(descriptor, 0, updated)
{}

inline void SamplerDescriptor::operator=(std::shared_ptr<const magma::Sampler> sampler) noexcept
{
    MAGMA_ASSERT(sampler);
    descriptor.sampler = *sampler;
    descriptor.imageView = VK_NULL_HANDLE;
    descriptor.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    updated = true;
}

inline ImageDescriptor::ImageDescriptor(VkDescriptorImageInfo& descriptor, VkImageType& imageType, VkImageUsageFlags requiredUsage, bool& updated) noexcept:
    BaseImageDescriptor(descriptor, imageType, requiredUsage, updated)
{}

inline void ImageDescriptor::operator=(std::shared_ptr<const ImageView> imageView) noexcept
{
    MAGMA_ASSERT(imageView);
    MAGMA_ASSERT(imageView->getImage()->getUsage() & requiredUsage);
    if (descriptor.imageView != *imageView)
    {
        setImageType(imageView);
        descriptor = imageView->getDescriptor(nullptr);
        updated = true;
    }
}

inline ImageSamplerDescriptor::ImageSamplerDescriptor(VkDescriptorImageInfo& descriptor, VkImageType& imageType, VkImageUsageFlags requiredUsage, bool& updated) noexcept:
    BaseImageDescriptor(descriptor, imageType, requiredUsage, updated)
{}

inline void ImageSamplerDescriptor::operator=(const ImageSampler& imageSampler) noexcept
{
    MAGMA_ASSERT(imageSampler.first);
    MAGMA_ASSERT(imageSampler.second);
    MAGMA_ASSERT(imageSampler.first->getImage()->getUsage() & requiredUsage);
    if ((descriptor.imageView != *imageSampler.first) ||
        (descriptor.sampler != *imageSampler.second))
    {
        setImageType(imageSampler.first);
        descriptor = imageSampler.first->getDescriptor(imageSampler.second);
        updated = true;
    }
}

inline ImageImmutableSamplerDescriptor::ImageImmutableSamplerDescriptor(VkDescriptorImageInfo& descriptor, VkSampler& immutableSampler, VkImageType& imageType, VkImageUsageFlags requiredUsage, bool& updated) noexcept:
    BaseImageDescriptor(descriptor, imageType, requiredUsage, updated),
    immutableSampler(immutableSampler)
{}

inline void ImageImmutableSamplerDescriptor::operator=(const ImageSampler& imageSampler) noexcept
{
    MAGMA_ASSERT(VK_NULL_HANDLE == immutableSampler);
    MAGMA_ASSERT(imageSampler.second);
    MAGMA_ASSERT(imageSampler.first->getImage()->getUsage() & requiredUsage);
    if (descriptor.imageView != *imageSampler.first)
    {
        setImageType(imageSampler.first);
        descriptor = imageSampler.first->getDescriptor(nullptr);
        updated = true;
    }
    if (VK_NULL_HANDLE == immutableSampler)
    {   // Immutable sampler must be updated only once
        immutableSampler = *imageSampler.second;
        updated = true;
    }
}

inline void ImageImmutableSamplerDescriptor::operator=(std::shared_ptr<const ImageView> imageView) noexcept
{   // Check that sampler is already set and stop carrying around it
    MAGMA_ASSERT(immutableSampler != VK_NULL_HANDLE);
    MAGMA_ASSERT(imageView);
    MAGMA_ASSERT(imageView->getImage()->getUsage() & requiredUsage);
    if (descriptor.imageView != *imageView)
    {
        setImageType(imageView);
        descriptor = imageView->getDescriptor(nullptr);
        updated = true;
    }
}
} // namespace array
} // namespace descriptor
} // namespace magma
