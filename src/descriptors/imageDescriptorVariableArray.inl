namespace magma::descriptor
{
inline ImageDescriptorVariableArray::ImageDescriptorVariableArray(VkDescriptorType descriptorType, uint32_t binding) noexcept:
    DescriptorSetLayoutBinding(descriptorType, 0, binding)
{}

inline void ImageDescriptorVariableArray::remove(uint32_t index)
{
    if (descriptors.empty())
        return;
    if (index == getSize() - 1)
    {
        descriptors.pop_back();
        --descriptorCount;
    }
    else
    {
        VkDescriptorImageInfo& descriptor = descriptors[index];
        descriptor.sampler = VK_NULL_HANDLE;
        descriptor.imageView = VK_NULL_HANDLE;
        invalidDescriptorIndices.insert(index);
    }
    dirty = true;
}

inline void ImageDescriptorVariableArray::clear()
{
    descriptors.clear();
    invalidDescriptorIndices.clear();
}

inline uint32_t ImageDescriptorVariableArray::insert(const VkDescriptorImageInfo& imageInfo)
{
    dirty = true;
    auto firstInvalid = invalidDescriptorIndices.begin();
    if (firstInvalid != invalidDescriptorIndices.end())
    {
        uint32_t index = *firstInvalid;
        descriptors[index] = imageInfo;
        invalidDescriptorIndices.erase(firstInvalid);
        return index;
    }
    descriptors.push_back(imageInfo);
    return descriptorCount++;
}

inline void ImageDescriptorVariableArray::write(VkDescriptorSet dstSet, VkWriteDescriptorSet& writeDescriptorSet) const noexcept
{
    writeDescriptor(dstSet, writeDescriptorSet);
    writeDescriptorSet.pImageInfo = descriptors.data();
    writeDescriptorSet.pBufferInfo = nullptr;
    writeDescriptorSet.pTexelBufferView = nullptr;
    dirty = false;
}

inline uint32_t CombinedImageSamplerVariableArray::add(lent_ptr<const ImageView> imageView,
    lent_ptr<const magma::Sampler> sampler)
{
    return insert(imageView->getDescriptor(sampler.get()));
}

inline uint32_t SampledImageVariableArray::add(lent_ptr<const ImageView> imageView)
{
    return insert(imageView->getDescriptor());
}
} // namespace magma::descriptor
