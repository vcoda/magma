class ImageDescriptor
{
    const VkImageUsageFlags requiredUsage;
    VkDescriptorImageInfo& imageDescriptor;
    bool& updated;

public:
    explicit ImageDescriptor(VkImageUsageFlags requiredUsage, VkDescriptorImageInfo& imageDescriptor, bool& updated) noexcept:
        requiredUsage(requiredUsage),
        imageDescriptor(imageDescriptor),
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
};
