namespace magma::descriptor
{
inline BufferDescriptor::BufferDescriptor(VkDescriptorType descriptorType, uint32_t binding) noexcept:
    DescriptorSetLayoutBinding(descriptorType, 1, binding),
    descriptor{VK_NULL_HANDLE, 0, 0}
{}

inline bool BufferDescriptor::resourceBinded() const noexcept
{
    return (descriptor.buffer != VK_NULL_HANDLE);
}

inline TexelBufferDescriptor::TexelBufferDescriptor(VkDescriptorType descriptorType, uint32_t binding) noexcept:
    DescriptorSetLayoutBinding(descriptorType, 1, binding),
    descriptor(VK_NULL_HANDLE)
{}

inline bool TexelBufferDescriptor::resourceBinded() const noexcept
{
    return (descriptor != VK_NULL_HANDLE);
}

inline UniformTexelBuffer& UniformTexelBuffer::operator=(lent_ptr<const BufferView> bufferView) noexcept
{
    update(std::move(bufferView), VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT);
    return *this;
}

inline StorageTexelBuffer& StorageTexelBuffer::operator=(lent_ptr<const BufferView> bufferView) noexcept
{
    update(std::move(bufferView), VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT);
    return *this;
}

inline UniformBuffer& UniformBuffer::operator=(lent_ptr<const Buffer> buffer) noexcept
{
    update(std::move(buffer), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
    return *this;
}

inline StorageBuffer& StorageBuffer::operator=(lent_ptr<const Buffer> buffer) noexcept
{
    update(std::move(buffer), VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
    return *this;
}

inline DynamicUniformBuffer& DynamicUniformBuffer::operator=(lent_ptr<const Buffer> buffer) noexcept
{
    update(std::move(buffer), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
    return *this;
}

inline DynamicStorageBuffer& DynamicStorageBuffer::operator=(lent_ptr<const Buffer> buffer) noexcept
{
    update(std::move(buffer), VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
    return *this;
}
} // namespace magma::descriptor
