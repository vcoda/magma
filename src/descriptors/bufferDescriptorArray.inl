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
    return std::all_of(this->descriptors.begin(), this->descriptors.end(),
        [](auto const& it)
        {
            return (it.buffer != VK_NULL_HANDLE);
        });
}

template<uint32_t Size>
inline BufferArrayElement BufferDescriptorArray<Size>::getElement(uint32_t index, VkBufferUsageFlags usage) noexcept
{
    return BufferArrayElement(this, this->descriptors[index], usage);
}

template<uint32_t Size>
inline void BufferDescriptorArray<Size>::write(VkDescriptorSet dstSet, VkWriteDescriptorSet& writeDescriptorSet) const noexcept
{
    MAGMA_ASSERT(associatedWithResource());
    writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSet.pNext = nullptr;
    writeDescriptorSet.dstSet = dstSet;
    writeDescriptorSet.dstBinding = this->binding;
    writeDescriptorSet.dstArrayElement = 0;
    writeDescriptorSet.descriptorCount = this->descriptorCount;
    writeDescriptorSet.descriptorType = this->descriptorType;
    writeDescriptorSet.pImageInfo = nullptr;
    writeDescriptorSet.pBufferInfo = this->descriptors.data();
    writeDescriptorSet.pTexelBufferView = nullptr;
    this->dirty = false;
}

template<uint32_t Size>
inline TexelBufferDescriptorArray<Size>::TexelBufferDescriptorArray(VkDescriptorType descriptorType, uint32_t binding) noexcept:
    DescriptorArray<VkBufferView, Size>(descriptorType, binding)
{}

template<uint32_t Size>
inline bool TexelBufferDescriptorArray<Size>::associatedWithResource() const noexcept
{
    return std::all_of(this->descriptors.begin(), this->descriptors.end(),
        [](auto view)
        {
            return (view != VK_NULL_HANDLE);
        });
}

template<uint32_t Size>
inline TexelBufferArrayElement TexelBufferDescriptorArray<Size>::getElement(uint32_t index, VkBufferUsageFlags usage) noexcept
{
    return TexelBufferArrayElement(this, this->descriptors[index], usage);
}

template<uint32_t Size>
inline void TexelBufferDescriptorArray<Size>::write(VkDescriptorSet dstSet, VkWriteDescriptorSet& writeDescriptorSet) const noexcept
{
    MAGMA_ASSERT(associatedWithResource());
    writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSet.pNext = nullptr;
    writeDescriptorSet.dstSet = dstSet;
    writeDescriptorSet.dstBinding = this->binding;
    writeDescriptorSet.dstArrayElement = 0;
    writeDescriptorSet.descriptorCount = this->descriptorCount;
    writeDescriptorSet.descriptorType = this->descriptorType;
    writeDescriptorSet.pImageInfo = nullptr;
    writeDescriptorSet.pBufferInfo = nullptr;
    writeDescriptorSet.pTexelBufferView = this->descriptors.data();
    this->dirty = false;
}

template<uint32_t Size>
inline TexelBufferArrayElement UniformTexelBufferArray<Size>::operator[](uint32_t index) noexcept
{
    return TexelBufferDescriptorArray<Size>::getElement(index, VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT);
}

template<uint32_t Size>
inline TexelBufferArrayElement StorageTexelBufferArray<Size>::operator[](uint32_t index) noexcept
{
    return TexelBufferDescriptorArray<Size>::getElement(index, VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT);
}

template<uint32_t Size>
inline BufferArrayElement UniformBufferArray<Size>::operator[](uint32_t index) noexcept
{
    return BufferDescriptorArray<Size>::getElement(index, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
}

template<uint32_t Size>
inline BufferArrayElement StorageBufferArray<Size>::operator[](uint32_t index) noexcept
{
    return BufferDescriptorArray<Size>::getElement(index, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
}

template<uint32_t Size>
inline BufferArrayElement DynamicUniformBufferArray<Size>::operator[](uint32_t index) noexcept
{
    return BufferDescriptorArray<Size>::getElement(index, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
}

template<uint32_t Size>
inline BufferArrayElement DynamicStorageBufferArray<Size>::operator[](uint32_t index) noexcept
{
    return BufferDescriptorArray<Size>::getElement(index, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
}
} // namespace descriptor
} // namespace magma
