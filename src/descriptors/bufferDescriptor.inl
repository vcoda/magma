namespace magma
{
namespace descriptor
{
inline BufferDescriptor::BufferDescriptor(VkDescriptorType descriptorType, uint32_t binding) noexcept:
    Descriptor<VkDescriptorBufferInfo>(descriptorType, binding)
{}

inline bool BufferDescriptor::associatedWithResource() const noexcept
{
    return (descriptor.buffer != VK_NULL_HANDLE);
}

inline TexelBufferDescriptor::TexelBufferDescriptor(VkDescriptorType descriptorType, uint32_t binding) noexcept:
    Descriptor<VkBufferView>(descriptorType, binding)
{}

inline bool TexelBufferDescriptor::associatedWithResource() const noexcept
{
    return (descriptor != VK_NULL_HANDLE);
}

inline UniformTexelBuffer& UniformTexelBuffer::operator=(std::shared_ptr<const BufferView> bufferView) noexcept
{
    updateBufferView(std::move(bufferView), VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT);
    return *this;
}

inline StorageTexelBuffer& StorageTexelBuffer::operator=(std::shared_ptr<const BufferView> bufferView) noexcept
{
    updateBufferView(std::move(bufferView), VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT);
    return *this;
}

inline UniformBuffer& UniformBuffer::operator=(std::shared_ptr<const Buffer> buffer) noexcept
{
    updateBuffer(std::move(buffer), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
    return *this;
}

inline StorageBuffer& StorageBuffer::operator=(std::shared_ptr<const Buffer> buffer) noexcept
{
    updateBuffer(std::move(buffer), VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
    return *this;
}

inline DynamicUniformBuffer& DynamicUniformBuffer::operator=(std::shared_ptr<const Buffer> buffer) noexcept
{
    updateBuffer(std::move(buffer), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
    return *this;
}

inline DynamicStorageBuffer& DynamicStorageBuffer::operator=(std::shared_ptr<const Buffer> buffer) noexcept
{
    updateBuffer(std::move(buffer), VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
    return *this;
}
} // namespace descriptor
} // namespace magma
