namespace magma
{
namespace descriptor
{
class DescriptorArray::ImageDescriptor
{
public:
    explicit ImageDescriptor(VkDescriptorImageInfo& imageDescriptor, VkImageType& imageType, VkImageUsageFlags requiredUsage, bool& updated) noexcept:
        imageDescriptor(imageDescriptor),
        imageType(imageType),
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
        std::shared_ptr<const Image> image = imageView->getImage();
        MAGMA_ASSERT(image->getUsage() & requiredUsage);
        if (imageDescriptor.imageView != *imageView)
        {
            imageDescriptor = imageView->getDescriptor(nullptr);
            if (imageType != VK_IMAGE_TYPE_MAX_ENUM)
                MAGMA_ASSERT(image->getType() == imageType);
            imageType = image->getType();
            updated = true;
        }
    }

    void operator=(const std::pair<std::shared_ptr<const ImageView>, std::shared_ptr<const magma::Sampler>>& combinedImageSampler) noexcept
    {
        MAGMA_ASSERT(combinedImageSampler.first);
        MAGMA_ASSERT(combinedImageSampler.second);
        std::shared_ptr<const Image> image = combinedImageSampler.first->getImage();
        MAGMA_ASSERT(image->getUsage() & requiredUsage);
        if ((imageDescriptor.imageView != *combinedImageSampler.first) ||
            (imageDescriptor.sampler != *combinedImageSampler.second))
        {
            imageDescriptor = combinedImageSampler.first->getDescriptor(combinedImageSampler.second);
            if (imageType != VK_IMAGE_TYPE_MAX_ENUM)
                MAGMA_ASSERT(image->getType() == imageType);
            imageType = image->getType();
            updated = true;
        }
    }

private:
    VkDescriptorImageInfo& imageDescriptor;
    VkImageType& imageType;
    const VkImageUsageFlags requiredUsage;
    bool& updated;
};
} // namespace descriptor
} // namespace magma
