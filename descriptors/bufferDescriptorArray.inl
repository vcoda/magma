namespace magma
{
namespace descriptor
{
template<uint32_t Size>
inline void BufferDescriptorArray<Size>::write(VkDescriptorSet dstSet, VkWriteDescriptorSet& writeDescriptorSet) const noexcept
{
    writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSet.pNext = nullptr;
    writeDescriptorSet.dstSet = dstSet;
    writeDescriptorSet.dstBinding = binding.binding;
    writeDescriptorSet.dstArrayElement = 0;
    writeDescriptorSet.descriptorCount = binding.descriptorCount;
    writeDescriptorSet.descriptorType = binding.descriptorType;
    writeDescriptorSet.pImageInfo = nullptr;
    writeDescriptorSet.pBufferInfo = descriptors.data();
    writeDescriptorSet.pTexelBufferView = nullptr;
    updated = false;
}

template<uint32_t Size>
inline DescriptorArray::BufferDescriptor BufferDescriptorArray<Size>::getArrayElement(uint32_t index, VkBufferUsageFlags requiredUsage) noexcept
{
    return BufferDescriptor(descriptors[index], requiredUsage, updated);
}

template<uint32_t Size>
inline void TexelBufferDescriptorArray<Size>::write(VkDescriptorSet dstSet, VkWriteDescriptorSet& writeDescriptorSet) const noexcept
{
    writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSet.pNext = nullptr;
    writeDescriptorSet.dstSet = dstSet;
    writeDescriptorSet.dstBinding = binding.binding;
    writeDescriptorSet.dstArrayElement = 0;
    writeDescriptorSet.descriptorCount = binding.descriptorCount;
    writeDescriptorSet.descriptorType = binding.descriptorType;
    writeDescriptorSet.pImageInfo = nullptr;
    writeDescriptorSet.pBufferInfo = nullptr;
    writeDescriptorSet.pTexelBufferView = views.data();
    updated = false;
}

template<uint32_t Size>
inline DescriptorArray::TexelBufferDescriptor TexelBufferDescriptorArray<Size>::getArrayElement(uint32_t index, VkBufferUsageFlags requiredUsage) noexcept
{
    return TexelBufferDescriptor(views[index], requiredUsage, updated);
}

template<uint32_t Size>
inline DescriptorArray::TexelBufferDescriptor UniformTexelBufferArray<Size>::operator[](uint32_t index) noexcept
{
    return this->getArrayElement(index, VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT);
}

template<uint32_t Size>
inline DescriptorArray::TexelBufferDescriptor StorageTexelBufferArray<Size>::operator[](uint32_t index) noexcept
{
    return this->getArrayElement(index, VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT);
}

template<uint32_t Size>
inline DescriptorArray::BufferDescriptor UniformBufferArray<Size>::operator[](uint32_t index) noexcept
{
    return this->getArrayElement(index, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
}

template<uint32_t Size>
inline DescriptorArray::BufferDescriptor StorageBufferArray<Size>::operator[](uint32_t index) noexcept
{
    return this->getArrayElement(index, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
}

template<uint32_t Size>
inline DescriptorArray::BufferDescriptor DynamicUniformBufferArray<Size>::operator[](uint32_t index) noexcept
{
    return this->getArrayElement(index, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
}

template<uint32_t Size>
inline DescriptorArray::BufferDescriptor DynamicStorageBufferArray<Size>::operator[](uint32_t index) noexcept
{
    return this->getArrayElement(index, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
}
} // namespace descriptor
} // namespace magma
