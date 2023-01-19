namespace magma
{
namespace descriptor
{
#ifdef VK_EXT_inline_uniform_block
template<class UniformBlockType>
inline InlineUniformBlock<UniformBlockType>::InlineUniformBlock(uint32_t binding) noexcept:
    Descriptor(VK_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK_EXT, sizeof(UniformBlockType), binding)
{}

template<class UniformBlockType>
inline VkWriteDescriptorSet InlineUniformBlock<UniformBlockType>::getWriteDescriptorSet(VkDescriptorSet dstSet) const noexcept
{
    VkWriteDescriptorSet writeDescriptorSet;
    VkWriteDescriptorSetInlineUniformBlockEXT writeDescriptorSetInlineUniformBlock;
    writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSet.pNext = &writeDescriptorSetInlineUniformBlock;
    writeDescriptorSet.dstSet = dstSet;
    writeDescriptorSet.dstBinding = binding.binding;
    writeDescriptorSet.dstArrayElement = 0;
    writeDescriptorSet.descriptorCount = sizeof(UniformBlockType); // The <descriptorCount> provides the total number of bytes a particular binding can hold
    writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK_EXT;
    writeDescriptorSet.pImageInfo = nullptr;
    writeDescriptorSet.pBufferInfo = nullptr;
    writeDescriptorSet.pTexelBufferView = nullptr;
    writeDescriptorSetInlineUniformBlock.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_INLINE_UNIFORM_BLOCK_EXT;
    writeDescriptorSetInlineUniformBlock.pNext = nullptr;
    writeDescriptorSetInlineUniformBlock.dataSize = sizeof(UniformBlockType);
    writeDescriptorSetInlineUniformBlock.pData = inlineUniformBlock;
    updated = false;
    return writeDescriptorSet;
}

template<class UniformBlockType>
inline InlineUniformBlock<UniformBlockType>& InlineUniformBlock<UniformBlockType>::operator=(const UniformBlockType& inlineUniformBlock_) noexcept
{
    inlineUniformBlock = &inlineUniformBlock_;
    updated = true;
    return *this;
}
#endif // VK_EXT_inline_uniform_block
} // namespace descriptor
} // namespace magma
