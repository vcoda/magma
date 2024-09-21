namespace magma::descriptor
{
inline ImageDescriptorVariableArray::ImageDescriptorVariableArray(VkDescriptorType descriptorType, uint32_t binding) noexcept:
    DescriptorSetLayoutBinding(descriptorType, 0, binding)
{}

inline void ImageDescriptorVariableArray::write(VkDescriptorSet dstSet, VkWriteDescriptorSet& writeDescriptorSet) const noexcept
{
    writeDescriptor(dstSet, writeDescriptorSet);
    writeDescriptorSet.pImageInfo = descriptors.data();
    writeDescriptorSet.pBufferInfo = nullptr;
    writeDescriptorSet.pTexelBufferView = nullptr;
    dirty = false;
}

inline void CombinedImageSamplerVariableArray::add(std::shared_ptr<const ImageView> imageView,
    std::shared_ptr<const magma::Sampler> sampler)
{
    descriptors.push_back(imageView->getDescriptor(std::move(sampler)));
    ++descriptorCount;
    dirty = true;
}

inline void SampledImageVariableArray::add(std::shared_ptr<const ImageView> imageView)
{
    descriptors.push_back(imageView->getDescriptor(nullptr));
    ++descriptorCount;
    dirty = true;
}
} // namespace magma::descriptor
