namespace magma::descriptor
{
inline BufferArrayElement::BufferArrayElement(DescriptorSetLayoutBinding *array,
    VkDescriptorBufferInfo& element, VkBufferUsageFlags usage) noexcept:
    ArrayElement<VkDescriptorBufferInfo>(array, element, usage)
{}

inline void BufferArrayElement::operator=(lent_ptr<const Buffer> buffer) noexcept
{
    MAGMA_ASSERT(buffer);
    MAGMA_ASSERT(MAGMA_BITWISE_AND(buffer->getUsage(), this->usage));
    if (this->element.buffer != *buffer)
    {   // TODO: offset, range?
        this->element = buffer->getDescriptor();
        this->setDirty();
    }
}

inline TexelBufferArrayElement::TexelBufferArrayElement(DescriptorSetLayoutBinding *array,
    VkBufferView& element, VkBufferUsageFlags usage) noexcept:
    ArrayElement<VkBufferView>(array, element, usage)
{}

inline void TexelBufferArrayElement::operator=(lent_ptr<const BufferView> bufferView) noexcept
{
    MAGMA_ASSERT(bufferView);
    MAGMA_ASSERT(MAGMA_BITWISE_AND(bufferView->getBuffer()->getUsage(), this->usage));
    if (this->element != *bufferView)
    {
        this->element = *bufferView;
        this->setDirty();
    }
}
} // namespace magma::descriptor
