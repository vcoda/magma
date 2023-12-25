namespace magma
{
namespace descriptor
{
template<uint32_t Size>
inline BufferDescriptorArray<Size>::BufferDescriptorArray(VkDescriptorType descriptorType, uint32_t binding) noexcept:
    DescriptorArray<VkDescriptorBufferInfo, Size>(descriptorType, binding)
{}

template<uint32_t Size>
inline bool BufferDescriptorArray<Size>::associatedWithResource() const noexcept
{
    return std::all_of(Super::descriptors.begin(), Super::descriptors.end(),
        [](const auto& it)
        {
            return (it.buffer != VK_NULL_HANDLE);
        });
}

template<uint32_t Size>
inline void BufferDescriptorArray<Size>::write(VkDescriptorSet dstSet, VkWriteDescriptorSet& writeDescriptorSet) const noexcept
{
    MAGMA_ASSERT(associatedWithResource());
    writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSet.pNext = nullptr;
    writeDescriptorSet.dstSet = dstSet;
    writeDescriptorSet.dstBinding = Super::binding.binding;
    writeDescriptorSet.dstArrayElement = 0;
    writeDescriptorSet.descriptorCount = Super::binding.descriptorCount;
    writeDescriptorSet.descriptorType = Super::binding.descriptorType;
    writeDescriptorSet.pImageInfo = nullptr;
    writeDescriptorSet.pBufferInfo = Super::descriptors.data();
    writeDescriptorSet.pTexelBufferView = nullptr;
    Super::updated = false;
}

template<uint32_t Size>
inline array::BufferDescriptor BufferDescriptorArray<Size>::getArrayElement(uint32_t index, VkBufferUsageFlags requiredUsage) noexcept
{
    return array::BufferDescriptor(Super::descriptors[index], requiredUsage, Super::updated);
}

template<uint32_t Size>
inline TexelBufferDescriptorArray<Size>::TexelBufferDescriptorArray(VkDescriptorType descriptorType, uint32_t binding) noexcept:
    DescriptorArray<VkBufferView, Size>(descriptorType, binding)
{}

template<uint32_t Size>
inline bool TexelBufferDescriptorArray<Size>::associatedWithResource() const noexcept
{
    return std::all_of(Super::descriptors.begin(), Super::descriptors.end(),
        [](auto view)
        {
            return (view != VK_NULL_HANDLE);
        });
}

template<uint32_t Size>
inline void TexelBufferDescriptorArray<Size>::write(VkDescriptorSet dstSet, VkWriteDescriptorSet& writeDescriptorSet) const noexcept
{
    MAGMA_ASSERT(associatedWithResource());
    writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSet.pNext = nullptr;
    writeDescriptorSet.dstSet = dstSet;
    writeDescriptorSet.dstBinding = Super::binding.binding;
    writeDescriptorSet.dstArrayElement = 0;
    writeDescriptorSet.descriptorCount = Super::binding.descriptorCount;
    writeDescriptorSet.descriptorType = Super::binding.descriptorType;
    writeDescriptorSet.pImageInfo = nullptr;
    writeDescriptorSet.pBufferInfo = nullptr;
    writeDescriptorSet.pTexelBufferView = Super::descriptors.data();
    Super::updated = false;
}

template<uint32_t Size>
inline array::TexelBufferDescriptor TexelBufferDescriptorArray<Size>::getArrayElement(uint32_t index, VkBufferUsageFlags requiredUsage) noexcept
{
    return array::TexelBufferDescriptor(Super::descriptors[index], requiredUsage, Super::updated);
}

template<uint32_t Size>
inline array::TexelBufferDescriptor UniformTexelBufferArray<Size>::operator[](uint32_t index) noexcept
{
    return TexelBufferDescriptorArray<Size>::getArrayElement(index, VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT);
}

template<uint32_t Size>
inline array::TexelBufferDescriptor StorageTexelBufferArray<Size>::operator[](uint32_t index) noexcept
{
    return TexelBufferDescriptorArray<Size>::getArrayElement(index, VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT);
}

template<uint32_t Size>
inline array::BufferDescriptor UniformBufferArray<Size>::operator[](uint32_t index) noexcept
{
    return TexelBufferDescriptorArray<Size>::getArrayElement(index, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
}

template<uint32_t Size>
inline array::BufferDescriptor StorageBufferArray<Size>::operator[](uint32_t index) noexcept
{
    return TexelBufferDescriptorArray<Size>::getArrayElement(index, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
}

template<uint32_t Size>
inline array::BufferDescriptor DynamicUniformBufferArray<Size>::operator[](uint32_t index) noexcept
{
    return TexelBufferDescriptorArray<Size>::getArrayElement(index, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
}

template<uint32_t Size>
inline array::BufferDescriptor DynamicStorageBufferArray<Size>::operator[](uint32_t index) noexcept
{
    return TexelBufferDescriptorArray<Size>::getArrayElement(index, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
}
} // namespace descriptor
} // namespace magma
