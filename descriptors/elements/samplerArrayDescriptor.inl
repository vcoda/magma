namespace magma
{
namespace descriptor
{
class DescriptorArray::ImmutableSamplerDescriptor
{
public:
    explicit ImmutableSamplerDescriptor(VkSampler& immutableSampler, bool& updated) noexcept:
        immutableSampler(immutableSampler),
        updated(updated)
    {}

    void operator=(std::shared_ptr<const magma::Sampler> sampler) noexcept
    {
        MAGMA_ASSERT(sampler);
        MAGMA_ASSERT(VK_NULL_HANDLE == immutableSampler);
        if (VK_NULL_HANDLE == immutableSampler) // Immutable sampler must be updated only once
        {
            immutableSampler = *sampler;
            updated = true;
        }
    }

protected:
    VkSampler& immutableSampler;
    bool& updated;
};

class DescriptorArray::ImageImmutableSamplerDescriptor : public DescriptorArray::ImmutableSamplerDescriptor
{
public:
    explicit ImageImmutableSamplerDescriptor(VkDescriptorImageInfo& imageDescriptor, VkSampler& immutableSampler, VkImageType& imageType, bool& updated) noexcept:
        ImmutableSamplerDescriptor(immutableSampler, updated),
        imageDescriptor(imageDescriptor),
        imageType(imageType)
    {}

    void operator=(const std::pair<std::shared_ptr<const ImageView>, std::shared_ptr<const magma::Sampler>>& combinedImageSampler) noexcept
    {
        MAGMA_ASSERT(combinedImageSampler.first);
        MAGMA_ASSERT(combinedImageSampler.second);
        std::shared_ptr<const Image> image = combinedImageSampler.first->getImage();
        MAGMA_ASSERT(image->getUsage() & VK_IMAGE_USAGE_SAMPLED_BIT);
        if (imageDescriptor.imageView != *combinedImageSampler.first)
        {
            imageDescriptor = combinedImageSampler.first->getDescriptor(nullptr);
            if (imageType != VK_IMAGE_TYPE_MAX_ENUM)
                MAGMA_ASSERT(image->getType() == imageType);
            imageType = image->getType();
            updated = true;
        }
        ImmutableSamplerDescriptor::operator=(combinedImageSampler.second);
    }

    void operator=(std::shared_ptr<const ImageView> imageView) noexcept
    {
        MAGMA_ASSERT(imageView);
        std::shared_ptr<const Image> image = imageView->getImage();
        MAGMA_ASSERT(image->getUsage() & VK_IMAGE_USAGE_SAMPLED_BIT);
        MAGMA_ASSERT(immutableSampler != VK_NULL_HANDLE); // Check that sampler is already set and stop carrying around it
        if (imageDescriptor.imageView != *imageView)
        {
            imageDescriptor = imageView->getDescriptor(nullptr);
            if (imageType != VK_IMAGE_TYPE_MAX_ENUM)
                MAGMA_ASSERT(image->getType() == imageType);
            imageType = image->getType();
            updated = true;
        }
    }

private:
    VkDescriptorImageInfo& imageDescriptor;
    VkImageType& imageType;
};
} // namespace descriptor
} // namespace magma
