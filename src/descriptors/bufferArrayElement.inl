namespace magma
{
namespace descriptor
{
inline BufferArrayElement::BufferArrayElement(DescriptorSetLayoutBinding *array,
    VkDescriptorBufferInfo& element, VkBufferUsageFlags usage) noexcept:
    ArrayElement(array),
    element(element),
    usage(usage)
{}

inline void BufferArrayElement::operator=(std::shared_ptr<const Buffer> buffer) noexcept
{
    MAGMA_ASSERT(buffer);
    MAGMA_ASSERT(buffer->getUsage() & usage);
    if (element.buffer != *buffer)
    {   // TODO: offset, range?
        element = buffer->getDescriptor();
        setDirty();
    }
}

inline TexelBufferArrayElement::TexelBufferArrayElement(DescriptorSetLayoutBinding *array,
    VkBufferView& element, VkBufferUsageFlags usage) noexcept:
    ArrayElement(array),
    element(element),
    usage(usage)
{}

inline void TexelBufferArrayElement::operator=(std::shared_ptr<const BufferView> bufferView) noexcept
{
    MAGMA_ASSERT(bufferView);
    MAGMA_ASSERT(bufferView->getBuffer()->getUsage() & usage);
    if (element != *bufferView)
    {
        element = *bufferView;
        setDirty();
    }
}
} // namespace descriptor
} // namespace magma
