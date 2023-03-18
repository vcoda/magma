namespace magma
{
namespace descriptor
{
template<uint32_t Size>
inline DescriptorArray::BufferDescriptor UniformTexelBufferArray<Size>::operator[](uint32_t index) noexcept
{
    return this->getBufferArrayElement(index, VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT);
}

template<uint32_t Size>
inline DescriptorArray::BufferDescriptor StorageTexelBufferArray<Size>::operator[](uint32_t index) noexcept
{
    return this->getBufferArrayElement(index, VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT);
}

template<uint32_t Size>
inline DescriptorArray::BufferDescriptor UniformBufferArray<Size>::operator[](uint32_t index) noexcept
{
    return this->getBufferArrayElement(index, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
}

template<uint32_t Size>
inline DescriptorArray::BufferDescriptor StorageBufferArray<Size>::operator[](uint32_t index) noexcept
{
    return this->getBufferArrayElement(index, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
}

template<uint32_t Size>
inline DescriptorArray::BufferDescriptor DynamicUniformBufferArray<Size>::operator[](uint32_t index) noexcept
{
    return this->getBufferArrayElement(index, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
}

template<uint32_t Size>
inline DescriptorArray::BufferDescriptor DynamicStorageBufferArray<Size>::operator[](uint32_t index) noexcept
{
    return this->getBufferArrayElement(index, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
}
} // namespace descriptor
} // namespace magma
