namespace magma
{
namespace descriptor
{
template<class UniformBlockType>
inline InlineUniformBlock<UniformBlockType>::InlineUniformBlock(uint32_t binding) noexcept:
    Descriptor(VK_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK_EXT, sizeof(UniformBlockType), binding)
{
    descriptor.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_INLINE_UNIFORM_BLOCK_EXT;
    descriptor.pNext = nullptr;
    descriptor.dataSize = sizeof(UniformBlockType);
    descriptor.pData = nullptr;
}

template<class UniformBlockType>
inline bool InlineUniformBlock<UniformBlockType>::associatedWithResource() const noexcept
{
    return (descriptor.pData != nullptr);
}

template<class UniformBlockType>
inline void InlineUniformBlock<UniformBlockType>::write(VkDescriptorSet dstSet, VkWriteDescriptorSet& writeDescriptorSet) const noexcept
{
    MAGMA_ASSERT(associatedWithResource());
    writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSet.pNext = &descriptor;
    writeDescriptorSet.dstSet = dstSet;
    writeDescriptorSet.dstBinding = binding.binding;
    writeDescriptorSet.dstArrayElement = 0;
    writeDescriptorSet.descriptorCount = sizeof(UniformBlockType); // The <descriptorCount> provides the total number of bytes a particular binding can hold
    writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK_EXT;
    writeDescriptorSet.pImageInfo = nullptr;
    writeDescriptorSet.pBufferInfo = nullptr;
    writeDescriptorSet.pTexelBufferView = nullptr;
    updated = false;
}

template<class UniformBlockType>
inline InlineUniformBlock<UniformBlockType>& InlineUniformBlock<UniformBlockType>::operator=(const UniformBlockType& inlineUniformBlock) noexcept
{
    if (descriptor.pData != &inlineUniformBlock)
    {
        descriptor.pData = &inlineUniformBlock;
        updated = true;
    }
    return *this;
}
} // namespace descriptor
} // namespace magma
