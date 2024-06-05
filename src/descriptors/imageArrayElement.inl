namespace magma
{
namespace descriptor
{
inline SamplerArrayElement::SamplerArrayElement(DescriptorSetLayoutBinding *array, VkDescriptorImageInfo& element) noexcept:
    ArrayElement(array),
    element(element)
{}

inline void SamplerArrayElement::operator=(std::shared_ptr<const magma::Sampler> sampler) noexcept
{
    MAGMA_ASSERT(sampler);
    element.sampler = *sampler;
    element.imageView = VK_NULL_HANDLE;
    element.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    setDirty();
}

inline ImageArrayElement::ImageArrayElement(DescriptorSetLayoutBinding *array, VkDescriptorImageInfo& element, VkImageUsageFlags usage) noexcept:
    ArrayElement(array),
    element(element),
    usage(usage)
{}

inline void ImageArrayElement::operator=(std::shared_ptr<const ImageView> imageView) noexcept
{
    MAGMA_ASSERT(imageView);
    MAGMA_ASSERT(imageView->getImage()->getUsage() & usage);
    if (element.imageView != *imageView)
    {
        element = imageView->getDescriptor(nullptr);
        setImageType(imageView->getImage()->getType());
        setDirty();
    }
}

inline ImageSamplerArrayElement::ImageSamplerArrayElement(DescriptorSetLayoutBinding *array, VkDescriptorImageInfo& element,
    VkImageUsageFlags usage) noexcept:
    ArrayElement(array),
    element(element),
    usage(usage)
{}

inline void ImageSamplerArrayElement::operator=(const ImageSampler& imageSampler) noexcept
{
    MAGMA_ASSERT(imageSampler.first);
    MAGMA_ASSERT(imageSampler.second);
    MAGMA_ASSERT(imageSampler.first->getImage()->getUsage() & usage);
    if ((element.imageView != *imageSampler.first) ||
        (element.sampler != *imageSampler.second))
    {
        element = imageSampler.first->getDescriptor(imageSampler.second);
        setImageType(imageSampler.first->getImage()->getType());
        setDirty();
    }
}

inline ImageImmutableSamplerArrayElement::ImageImmutableSamplerArrayElement(DescriptorSetLayoutBinding *array,
    VkDescriptorImageInfo& element, VkSampler& immutableSampler, VkImageUsageFlags usage) noexcept:
    ArrayElement(array),
    element(element),
    immutableSampler(immutableSampler),
    usage(usage)
{}

inline void ImageImmutableSamplerArrayElement::operator=(const ImageSampler& imageSampler) noexcept
{
    MAGMA_ASSERT(imageSampler.second);
    MAGMA_ASSERT(imageSampler.first->getImage()->getUsage() & usage);
    if (element.imageView != *imageSampler.first)
    {
        element = imageSampler.first->getDescriptor(nullptr);
        setImageType(imageSampler.first->getImage()->getType());
        setDirty();
    }
    MAGMA_ASSERT(VK_NULL_HANDLE == immutableSampler);
    if (VK_NULL_HANDLE == immutableSampler)
    {   // Immutable sampler must be updated only once
        immutableSampler = *imageSampler.second;
        setDirty();
    }
}

inline void ImageImmutableSamplerArrayElement::operator=(std::shared_ptr<const ImageView> imageView) noexcept
{   // Check that sampler is already set and stop carrying around it
    MAGMA_ASSERT(immutableSampler != VK_NULL_HANDLE);
    MAGMA_ASSERT(imageView);
    MAGMA_ASSERT(imageView->getImage()->getUsage() & usage);
    if (element.imageView != *imageView)
    {
        element = imageView->getDescriptor(nullptr);
        setImageType(imageView->getImage()->getType());
        setDirty();
    }
}
} // namespace descriptor
} // namespace magma
