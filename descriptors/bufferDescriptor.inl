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

inline void BufferDescriptor::write(VkDescriptorSet dstSet, VkWriteDescriptorSet& writeDescriptorSet) const noexcept
{
    MAGMA_ASSERT(associatedWithResource());
    writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSet.pNext = nullptr;
    writeDescriptorSet.dstSet = dstSet;
    writeDescriptorSet.dstBinding = binding.binding;
    writeDescriptorSet.dstArrayElement = 0;
    writeDescriptorSet.descriptorCount = binding.descriptorCount;
    writeDescriptorSet.descriptorType = binding.descriptorType;
    writeDescriptorSet.pImageInfo = nullptr;
    writeDescriptorSet.pBufferInfo = &descriptor;
    writeDescriptorSet.pTexelBufferView = nullptr;
    updated = false;
}

inline void BufferDescriptor::updateBuffer(std::shared_ptr<const Buffer> buffer, VkBufferUsageFlags requiredUsage) noexcept
{
    MAGMA_UNUSED(requiredUsage);
    MAGMA_ASSERT(buffer);
    MAGMA_ASSERT(buffer->getUsage() & requiredUsage);
    if (descriptor.buffer != *buffer)
    {   // TODO: offset, range?
        descriptor = buffer->getDescriptor();
        updated = true;
    }
}

inline TexelBufferDescriptor::TexelBufferDescriptor(VkDescriptorType descriptorType, uint32_t binding) noexcept:
    Descriptor<VkBufferView>(descriptorType, binding)
{}

inline bool TexelBufferDescriptor::associatedWithResource() const noexcept
{
    return (descriptor != VK_NULL_HANDLE);
}

inline void TexelBufferDescriptor::write(VkDescriptorSet dstSet, VkWriteDescriptorSet& writeDescriptorSet) const noexcept
{
    MAGMA_ASSERT(associatedWithResource());
    writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSet.pNext = nullptr;
    writeDescriptorSet.dstSet = dstSet;
    writeDescriptorSet.dstBinding = binding.binding;
    writeDescriptorSet.dstArrayElement = 0;
    writeDescriptorSet.descriptorCount = binding.descriptorCount;
    writeDescriptorSet.descriptorType = binding.descriptorType;
    writeDescriptorSet.pImageInfo = nullptr;
    writeDescriptorSet.pBufferInfo = nullptr;
    writeDescriptorSet.pTexelBufferView = &descriptor;
    updated = false;
}

inline void TexelBufferDescriptor::updateBufferView(std::shared_ptr<const BufferView> bufferView, VkBufferUsageFlags requiredUsage) noexcept
{
    MAGMA_UNUSED(requiredUsage);
    MAGMA_ASSERT(bufferView);
    MAGMA_ASSERT(bufferView->getBuffer()->getUsage() & requiredUsage);
    if (descriptor != *bufferView)
    {
        descriptor = *bufferView;
        updated = true;
    }
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
