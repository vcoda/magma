#include "elements/samplerArrayDescriptor.inl"

namespace magma
{
namespace descriptor
{
template<uint32_t Size>
inline DescriptorArray::ImageDescriptor SamplerArray<Size>::operator[](uint32_t index) noexcept
{
    return this->getImageArrayElement(index, 0);
}

template<uint32_t Size>
inline DescriptorArray::ImmutableSamplerDescriptor ImmutableSamplerArray<Size>::operator[](uint32_t index) noexcept
{
    return ImmutableSamplerDescriptor(immutableSamplers[index], updated);
}

template<uint32_t Size>
inline DescriptorArray::ImageDescriptor CombinedImageSamplerArray<Size>::operator[](uint32_t index) noexcept
{
    return this->getImageArrayElement(index, VK_IMAGE_USAGE_SAMPLED_BIT);
}

template<uint32_t Size>
inline DescriptorArray::ImageImmutableSamplerDescriptor CombinedImageImmutableSamplerArray<Size>::operator[](uint32_t index) noexcept
{
    return ImageImmutableSamplerDescriptor(imageDescriptors[index], immutableSamplers[index], imageType, updated);
}

template<uint32_t Size>
inline DescriptorArray::ImageDescriptor SampledImageArray<Size>::operator[](uint32_t index) noexcept
{
    return this->getImageArrayElement(index, VK_IMAGE_USAGE_SAMPLED_BIT);
}

template<uint32_t Size>
inline DescriptorArray::ImageDescriptor StorageImageArray<Size>::operator[](uint32_t index) noexcept
{
    return this->getImageArrayElement(index, VK_IMAGE_USAGE_STORAGE_BIT);
}

template<uint32_t Size>
inline DescriptorArray::ImageDescriptor InputAttachmentArray<Size>::operator[](uint32_t index) noexcept
{
    return this->getImageArrayElement(index, VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT);
}
} // namespace descriptor
} // namespace magma
