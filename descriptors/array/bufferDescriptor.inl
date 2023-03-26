namespace magma
{
namespace descriptor
{
namespace array
{
inline BufferDescriptor::BufferDescriptor(VkDescriptorBufferInfo& descriptor, VkBufferUsageFlags requiredUsage, bool& updated) noexcept:
    Descriptor<VkDescriptorBufferInfo>(descriptor, requiredUsage, updated)
{}

inline void BufferDescriptor::operator=(std::shared_ptr<const Buffer> buffer) noexcept
{
    MAGMA_ASSERT(buffer);
    MAGMA_ASSERT(buffer->getUsage() & requiredUsage);
    if (descriptor.buffer != *buffer)
    {   // TODO: offset, range?
        descriptor = buffer->getDescriptor();
        updated = true;
    }
}

inline TexelBufferDescriptor::TexelBufferDescriptor(VkBufferView& descriptor, VkBufferUsageFlags requiredUsage, bool& updated) noexcept:
    Descriptor<VkBufferView>(descriptor, requiredUsage, updated)
{}

inline void TexelBufferDescriptor::operator=(std::shared_ptr<const BufferView> bufferView) noexcept
{
    MAGMA_ASSERT(bufferView);
    MAGMA_ASSERT(bufferView->getBuffer()->getUsage() & requiredUsage);
    if (descriptor != *bufferView)
    {
        descriptor = *bufferView;
        updated = true;
    }
}
} // namespace array
} // namespace descriptor
} // namespace magma
