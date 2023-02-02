class ImageDescriptor
{
    VkDescriptorImageInfo& imageDescriptor;
    const VkImageUsageFlags requiredUsage;
    bool& updated;

public:
    explicit ImageDescriptor(VkDescriptorImageInfo& imageDescriptor, VkImageUsageFlags requiredUsage, bool& updated) noexcept:
        imageDescriptor(imageDescriptor),
        requiredUsage(requiredUsage),
        updated(updated)
    {}

    void operator=(std::shared_ptr<const magma::Sampler> sampler) noexcept
    {
        const VkSampler handle = MAGMA_OPTIONAL_HANDLE(sampler);
        if (imageDescriptor.sampler != handle)
        {
            imageDescriptor.sampler = handle;
            imageDescriptor.imageView = VK_NULL_HANDLE;
            imageDescriptor.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            updated = true;
        }
    }

    void operator=(std::shared_ptr<const ImageView> imageView) noexcept
    {
        MAGMA_ASSERT(imageView);
        MAGMA_ASSERT(imageView->getImage()->getUsage() & requiredUsage);
        if (imageDescriptor.imageView != *imageView)
        {
            imageDescriptor = imageView->getDescriptor(nullptr);
            updated = true;
        }
    }

    void operator=(const std::pair<std::shared_ptr<const ImageView>, std::shared_ptr<const magma::Sampler>>& combinedImageSampler) noexcept
    {
        MAGMA_ASSERT(combinedImageSampler.first);
        MAGMA_ASSERT(combinedImageSampler.second);
        MAGMA_ASSERT(combinedImageSampler.first->getImage()->getUsage() & requiredUsage);
        if ((imageDescriptor.imageView != *combinedImageSampler.first) ||
            (imageDescriptor.sampler != *combinedImageSampler.second))
        {
            imageDescriptor = combinedImageSampler.first->getDescriptor(combinedImageSampler.second);
            updated = true;
        }
    }
};
