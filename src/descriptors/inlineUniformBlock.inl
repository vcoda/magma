namespace magma
{
namespace descriptor
{
template<class UniformBlockType>
inline InlineUniformBlock<UniformBlockType>::InlineUniformBlock(uint32_t binding) noexcept:
    DescriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK_EXT, sizeof(UniformBlockType), binding),
    descriptor{
        VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_INLINE_UNIFORM_BLOCK_EXT,
        nullptr,
        sizeof(UniformBlockType),
        nullptr
    }
{}

template<class UniformBlockType>
inline bool InlineUniformBlock<UniformBlockType>::associatedWithResource() const noexcept
{
    return (descriptor.pData != nullptr);
}

template<class UniformBlockType>
inline void InlineUniformBlock<UniformBlockType>::write(VkDescriptorSet dstSet, VkWriteDescriptorSet& writeDescriptorSet) const noexcept
{
    writeDescriptor(dstSet, writeDescriptorSet);
    writeDescriptorSet.pNext = &descriptor;
    writeDescriptorSet.pImageInfo = nullptr;
    writeDescriptorSet.pBufferInfo = nullptr;
    writeDescriptorSet.pTexelBufferView = nullptr;
    dirty = false;
}

template<class UniformBlockType>
inline InlineUniformBlock<UniformBlockType>& InlineUniformBlock<UniformBlockType>::operator=(const UniformBlockType& inlineUniformBlock) noexcept
{
    if (descriptor.pData != &inlineUniformBlock)
    {
        descriptor.pData = &inlineUniformBlock;
        dirty = true;
    }
    return *this;
}
} // namespace descriptor
} // namespace magma
