namespace magma::descriptor
{
template<uint32_t Size>
inline ImageDescriptorArray<Size>::ImageDescriptorArray(VkDescriptorType descriptorType, uint32_t binding) noexcept:
    DescriptorSetLayoutBinding(descriptorType, Size, binding)
{}

template<uint32_t Size>
inline bool ImageDescriptorArray<Size>::associatedWithResource() const noexcept
{
    switch (descriptorType)
    {
    case VK_DESCRIPTOR_TYPE_SAMPLER:
        return std::all_of(descriptors.begin(), descriptors.end(),
            [](auto const& it)
            {
                return (it.sampler != VK_NULL_HANDLE);
            });
    case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
        return std::all_of(descriptors.begin(), descriptors.end(),
            [](auto const& it)
            {
                return (it.sampler != VK_NULL_HANDLE) && (it.imageView != VK_NULL_HANDLE);
            });
    case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE:
    case VK_DESCRIPTOR_TYPE_STORAGE_IMAGE:
    case VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT:
        return std::all_of(descriptors.begin(), descriptors.end(),
            [](auto const& it)
            {
                return (it.imageView != VK_NULL_HANDLE);
            });
    default:
        return false;
    }
}

template<uint32_t Size>
inline ImageArrayElement ImageDescriptorArray<Size>::getElement(uint32_t index, VkImageUsageFlags usage) noexcept
{
    return ImageArrayElement(this, descriptors[index], usage);
}

template<uint32_t Size>
inline void ImageDescriptorArray<Size>::write(VkDescriptorSet dstSet, VkWriteDescriptorSet& writeDescriptorSet) const noexcept
{
    writeDescriptor(dstSet, writeDescriptorSet);
    writeDescriptorSet.pImageInfo = descriptors.data();
    writeDescriptorSet.pBufferInfo = nullptr;
    writeDescriptorSet.pTexelBufferView = nullptr;
    dirty = false;
}

template<uint32_t Size>
inline SamplerArrayElement SamplerArray<Size>::operator[](uint32_t index) noexcept
{
    return SamplerArrayElement(this, this->descriptors[index]);
}

template<uint32_t Size>
inline ImageSamplerArrayElement CombinedImageSamplerArray<Size>::operator[](uint32_t index) noexcept
{
    return ImageSamplerArrayElement(this, this->descriptors[index], VK_IMAGE_USAGE_SAMPLED_BIT);
}

template<uint32_t Size>
inline bool CombinedImageImmutableSamplerArray<Size>::associatedWithResource() const noexcept
{
    const bool associatedWithSamplers = std::all_of(this->descriptors.begin(), this->descriptors.end(),
        [](auto const& it)
        {
            return (it.sampler != VK_NULL_HANDLE);
        }) || std::all_of(immutableSamplers.begin(), immutableSamplers.end(),
        [](VkSampler it)
        {
            return (it != VK_NULL_HANDLE);
        });
    return std::all_of(this->descriptors.begin(), this->descriptors.end(),
        [](auto const& it)
        {
            return (it.imageView != VK_NULL_HANDLE);
        }) && associatedWithSamplers;
}

template<uint32_t Size>
inline CombinedImageImmutableSamplerArray<Size>::CombinedImageImmutableSamplerArray(uint32_t binding) noexcept:
    ImageDescriptorArray<Size>(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, binding)
{   // If pImmutableSamplers is not NULL, then it is a pointer
    // to an array of sampler handles that will be copied
    // into the set layout and used for the corresponding binding.
    this->pImmutableSamplers = immutableSamplers.data();
}

template<uint32_t Size>
inline ImageImmutableSamplerArrayElement CombinedImageImmutableSamplerArray<Size>::operator[](uint32_t index) noexcept
{
    return ImageImmutableSamplerArrayElement(this, this->descriptors[index], immutableSamplers[index], VK_IMAGE_USAGE_SAMPLED_BIT);
}

template<uint32_t Size>
inline ImageArrayElement SampledImageArray<Size>::operator[](uint32_t index) noexcept
{
    return ImageDescriptorArray<Size>::getElement(index, VK_IMAGE_USAGE_SAMPLED_BIT);
}

template<uint32_t Size>
inline ImageArrayElement StorageImageArray<Size>::operator[](uint32_t index) noexcept
{
    return ImageDescriptorArray<Size>::getElement(index, VK_IMAGE_USAGE_STORAGE_BIT);
}

template<uint32_t Size>
inline ImageArrayElement InputAttachmentArray<Size>::operator[](uint32_t index) noexcept
{
    return ImageDescriptorArray<Size>::getElement(index, VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT);
}
} // namespace magma::descriptor
