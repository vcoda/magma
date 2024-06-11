namespace magma
{
namespace descriptor
{
template<uint32_t Size>
inline BufferDescriptorArray<Size>::BufferDescriptorArray(VkDescriptorType descriptorType, uint32_t binding) noexcept:
    DescriptorSetLayoutBinding(descriptorType, Size, binding)
{}

template<uint32_t Size>
inline bool BufferDescriptorArray<Size>::associatedWithResource() const noexcept
{
    return std::all_of(descriptors.begin(), descriptors.end(),
        [](auto const& it)
        {
            return (it.buffer != VK_NULL_HANDLE);
        });
}

template<uint32_t Size>
inline BufferArrayElement BufferDescriptorArray<Size>::getElement(uint32_t index, VkBufferUsageFlags usage) noexcept
{
    return BufferArrayElement(this, descriptors[index], usage);
}

template<uint32_t Size>
inline void BufferDescriptorArray<Size>::write(VkDescriptorSet dstSet, VkWriteDescriptorSet& writeDescriptorSet) const noexcept
{
    writeDescriptor(dstSet, writeDescriptorSet);
    writeDescriptorSet.pImageInfo = nullptr;
    writeDescriptorSet.pBufferInfo = descriptors.data();
    writeDescriptorSet.pTexelBufferView = nullptr;
    dirty = false;
}

template<uint32_t Size>
inline TexelBufferDescriptorArray<Size>::TexelBufferDescriptorArray(VkDescriptorType descriptorType, uint32_t binding) noexcept:
    DescriptorSetLayoutBinding(descriptorType, Size, binding)
{}

template<uint32_t Size>
inline bool TexelBufferDescriptorArray<Size>::associatedWithResource() const noexcept
{
    return std::all_of(descriptors.begin(), descriptors.end(),
        [](auto view)
        {
            return (view != VK_NULL_HANDLE);
        });
}

template<uint32_t Size>
inline TexelBufferArrayElement TexelBufferDescriptorArray<Size>::getElement(uint32_t index, VkBufferUsageFlags usage) noexcept
{
    return TexelBufferArrayElement(this, descriptors[index], usage);
}

template<uint32_t Size>
inline void TexelBufferDescriptorArray<Size>::write(VkDescriptorSet dstSet, VkWriteDescriptorSet& writeDescriptorSet) const noexcept
{
    writeDescriptor(dstSet, writeDescriptorSet);
    writeDescriptorSet.pImageInfo = nullptr;
    writeDescriptorSet.pBufferInfo = nullptr;
    writeDescriptorSet.pTexelBufferView = descriptors.data();
    dirty = false;
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
