class ImmutableSamplerDescriptor
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

class ImageImmutableSamplerDescriptor : public ImmutableSamplerDescriptor
{
    VkDescriptorImageInfo& imageDescriptor;

public:
    explicit ImageImmutableSamplerDescriptor(VkDescriptorImageInfo& imageDescriptor, VkSampler& immutableSampler, bool& updated) noexcept:
        ImmutableSamplerDescriptor(immutableSampler, updated),
        imageDescriptor(imageDescriptor)
    {}

    void operator=(const std::pair<std::shared_ptr<const ImageView>, std::shared_ptr<const magma::Sampler>>& combinedImageSampler) noexcept
    {
        MAGMA_ASSERT(combinedImageSampler.first);
        MAGMA_ASSERT(combinedImageSampler.second);
        MAGMA_ASSERT(combinedImageSampler.first->getImage()->getUsage() & VK_IMAGE_USAGE_SAMPLED_BIT);
        if (imageDescriptor.imageView != *combinedImageSampler.first)
        {
            imageDescriptor = combinedImageSampler.first->getDescriptor(nullptr);
            updated = true;
        }
        ImmutableSamplerDescriptor::operator=(combinedImageSampler.second);
    }

    void operator=(std::shared_ptr<const ImageView> imageView) noexcept
    {
        MAGMA_ASSERT(imageView);
        MAGMA_ASSERT(imageView->getImage()->getUsage() & VK_IMAGE_USAGE_SAMPLED_BIT);
        MAGMA_ASSERT(immutableSampler != VK_NULL_HANDLE); // Check that sampler is already set and stop carrying around it
        if (imageDescriptor.imageView != *imageView)
        {
            imageDescriptor = imageView->getDescriptor(nullptr);
            updated = true;
        }
    }
};
