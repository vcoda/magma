namespace magma
{
namespace descriptor
{
namespace array
{
template<class Type>
inline Descriptor<Type>::Descriptor(Type& descriptor, VkFlags requiredUsage, bool& updated) noexcept:
    descriptor(descriptor),
    requiredUsage(requiredUsage),
    updated(updated)
{}

inline BaseImageDescriptor::BaseImageDescriptor(VkDescriptorImageInfo& descriptor, VkImageType& imageType, VkFlags requiredUsage, bool& updated) noexcept:
    Descriptor<VkDescriptorImageInfo>(descriptor, requiredUsage, updated),
    imageType(imageType)
{}

inline void BaseImageDescriptor::setImageType(std::shared_ptr<const ImageView> imageView) noexcept
{
    if (imageType != VK_IMAGE_TYPE_MAX_ENUM)
    {   // Descriptor should have the same image type
        MAGMA_ASSERT(imageView->getImage()->getType() == imageType);
    }
    imageType = imageView->getImage()->getType();
}
} // namespace array
} // namespace descriptor
} // namespace magma
