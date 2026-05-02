namespace magma::descriptor
{
inline VariableCountImageDescriptors::VariableCountImageDescriptors(VkDescriptorType descriptorType, uint32_t binding) noexcept:
#ifdef VK_EXT_descriptor_indexing
    DescriptorSetLayoutBinding(descriptorType, 0, binding, VK_DESCRIPTOR_BINDING_VARIABLE_DESCRIPTOR_COUNT_BIT_EXT)
#else
    DescriptorSetLayoutBinding(descriptorType, 0, binding)
#endif
{}

inline void VariableCountImageDescriptors::reserve(std::size_t capacity)
{
    descriptors.reserve(capacity);
}

inline bool VariableCountImageDescriptors::erase(BindlessHandle handle)
{
    bool erased = descriptors.erase(handle);
    if (erased)
    {
        dirty = true;
        --descriptorCount;
    }
    return erased;
}

inline void VariableCountImageDescriptors::clear()
{
    descriptors.clear();
    descriptorCount = 0;
    dirty = true;
}

inline void VariableCountImageDescriptors::write(VkDescriptorSet dstSet, VkWriteDescriptorSet& writeDescriptorSet) const noexcept
{
    writeDescriptor(dstSet, writeDescriptorSet);
    writeDescriptorSet.pImageInfo = descriptors.data();
    writeDescriptorSet.pBufferInfo = nullptr;
    writeDescriptorSet.pTexelBufferView = nullptr;
    dirty = false;
}

inline BindlessHandle VariableCountImageDescriptors::insert(const ImageView *imageView, const magma::Sampler *sampler)
{
    VkDescriptorImageInfo descriptor = imageView->getDescriptor(sampler);
    if (VK_IMAGE_LAYOUT_UNDEFINED == descriptor.imageLayout)
    {   // Assume that later image layout will be changed,
        // e.g. when a render pass instance ends.
        auto image = imageView->getImage();
        VkImageUsageFlags usage = image->getUsage();
        if (usage & VK_IMAGE_USAGE_SAMPLED_BIT)
            descriptor.imageLayout = image->getOptimalReadOnlyLayout();
        else if (usage & VK_IMAGE_USAGE_STORAGE_BIT)
            descriptor.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
    }
    BindlessHandle handle = descriptors.insert(descriptor);
    ++descriptorCount;
    dirty = true;
    return handle;
}

inline BindlessHandle VariableCountCombinedImageSamplers::insert(lent_ptr<const ImageView> imageView, lent_ptr<const magma::Sampler> sampler)
{
    return VariableCountImageDescriptors::insert(imageView.get(), sampler.get());
}

inline BindlessHandle VariableCountSampledImages::insert(lent_ptr<const ImageView> imageView)
{
    return VariableCountImageDescriptors::insert(imageView.get(), nullptr);
}
} // namespace magma::descriptor
