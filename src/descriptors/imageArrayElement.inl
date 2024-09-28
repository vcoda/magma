namespace magma::descriptor
{
inline SamplerArrayElement::SamplerArrayElement(DescriptorSetLayoutBinding *array, VkDescriptorImageInfo& element) noexcept:
    ArrayElement<VkDescriptorImageInfo>(array, element, 0)
{}

inline void SamplerArrayElement::operator=(std::shared_ptr<const magma::Sampler> sampler) noexcept
{
    MAGMA_ASSERT(sampler);
    this->element.sampler = *sampler;
    this->element.imageView = VK_NULL_HANDLE;
    this->element.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    this->setDirty();
}

inline ImageArrayElement::ImageArrayElement(DescriptorSetLayoutBinding *array, VkDescriptorImageInfo& element, VkImageUsageFlags usage) noexcept:
    ArrayElement<VkDescriptorImageInfo>(array, element, usage)
{}

inline void ImageArrayElement::operator=(std::shared_ptr<const ImageView> imageView) noexcept
{
    MAGMA_ASSERT(imageView);
    MAGMA_ASSERT(MAGMA_BITWISE_AND(imageView->getImage()->getUsage(), this->usage));
    if (this->element.imageView != *imageView)
    {
        this->element = imageView->getDescriptor();
        this->setImageType(imageView->getImage()->getType());
        this->setDirty();
    }
}

inline ImageSamplerArrayElement::ImageSamplerArrayElement(DescriptorSetLayoutBinding *array, VkDescriptorImageInfo& element, VkImageUsageFlags usage) noexcept:
    ArrayElement<VkDescriptorImageInfo>(array, element, usage)
{}

inline void ImageSamplerArrayElement::operator=(const ImageSampler& imageSampler) noexcept
{
    MAGMA_ASSERT(imageSampler.first);
    MAGMA_ASSERT(imageSampler.second);
    MAGMA_ASSERT(MAGMA_BITWISE_AND(imageSampler.first->getImage()->getUsage(), this->usage));
    if ((this->element.imageView != *imageSampler.first) ||
        (this->element.sampler != *imageSampler.second))
    {
        this->element = imageSampler.first->getDescriptor(imageSampler.second.get());
        this->setImageType(imageSampler.first->getImage()->getType());
        this->setDirty();
    }
}

inline ImageImmutableSamplerArrayElement::ImageImmutableSamplerArrayElement(DescriptorSetLayoutBinding *array,
    VkDescriptorImageInfo& element, VkSampler& immutableSampler, VkImageUsageFlags usage) noexcept:
    ArrayElement<VkDescriptorImageInfo>(array, element, usage),
    immutableSampler(immutableSampler)
{}

inline void ImageImmutableSamplerArrayElement::operator=(const ImageSampler& imageSampler) noexcept
{
    MAGMA_ASSERT(imageSampler.second);
    MAGMA_ASSERT(MAGMA_BITWISE_AND(imageSampler.first->getImage()->getUsage(), this->usage));
    if (this->element.imageView != *imageSampler.first)
    {
        this->element = imageSampler.first->getDescriptor();
        this->setImageType(imageSampler.first->getImage()->getType());
        this->setDirty();
    }
    MAGMA_ASSERT(VK_NULL_HANDLE == immutableSampler);
    if (VK_NULL_HANDLE == immutableSampler)
    {   // Immutable sampler must be updated only once
        immutableSampler = *imageSampler.second;
        this->setDirty();
    }
}

inline void ImageImmutableSamplerArrayElement::operator=(std::shared_ptr<const ImageView> imageView) noexcept
{   // Check that sampler is already set and stop carrying around it
    MAGMA_ASSERT(immutableSampler != VK_NULL_HANDLE);
    MAGMA_ASSERT(imageView);
    MAGMA_ASSERT(MAGMA_BITWISE_AND(imageView->getImage()->getUsage(), this->usage));
    if (this->element.imageView != *imageView)
    {
        this->element = imageView->getDescriptor();
        this->setImageType(imageView->getImage()->getType());
        this->setDirty();
    }
}
} // namespace magma::descriptor
