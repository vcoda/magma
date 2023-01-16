namespace magma
{
namespace binding
{
#ifdef VK_EXT_inline_uniform_block
template<typename UniformBlockType>
inline InlineUniformBlock<UniformBlockType>::InlineUniformBlock(uint32_t binding) noexcept:
    DescriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK_EXT, sizeof(UniformBlockType), binding)
{}

template<typename UniformBlockType>
inline InlineUniformBlock<UniformBlockType>::InlineUniformBlock(const InlineUniformBlock<UniformBlockType>& other) noexcept
{
    binding = other.binding;
    descriptorType = other.descriptorType;
    descriptorCount = other.descriptorCount;
    stageFlags = other.stageFlags;
    pImmutableSamplers = other.pImmutableSampler;
    updated = other.updated;
    writeDescriptorSetInlineUniformBlock.sType = other.writeDescriptorSetInlineUniformBlock.sType;
    writeDescriptorSetInlineUniformBlock.pNext = other.writeDescriptorSetInlineUniformBlock.pNext;
    writeDescriptorSetInlineUniformBlock.dataSize = other.writeDescriptorSetInlineUniformBlock.dataSize;
    writeDescriptorSetInlineUniformBlock.pData = new(std::nothrow) UniformBlockType();
    if (writeDescriptorSetInlineUniformBlock.pData)
        memcpy(writeDescriptorSetInlineUniformBlock.pData, other.writeDescriptorSetInlineUniformBlock.pData, sizeof(UniformBlockType));
}

template<typename UniformBlockType>
inline InlineUniformBlock<UniformBlockType>::~InlineUniformBlock()
{
    delete reinterpret_cast<UniformBlockType *>(writeDescriptorSetInlineUniformBlock.pData);
}

template<typename UniformBlockType>
inline VkWriteDescriptorSet InlineUniformBlock<UniformBlockType>::getWriteDescriptorSet(VkDescriptorSet dstSet) const noexcept
{
    VkWriteDescriptorSet writeDescriptorSet;
    writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    writeDescriptorSet.pNext = &writeDescriptorSetInlineUniformBlock;
    writeDescriptorSet.dstSet = dstSet;
    writeDescriptorSet.dstBinding = binding;
    writeDescriptorSet.dstArrayElement = 0;
    writeDescriptorSet.descriptorCount = sizeof(UniformBlockType); // The <descriptorCount> provides the total number of bytes a particular binding can hold
    writeDescriptorSet.descriptorType = descriptorType;
    writeDescriptorSet.pImageInfo = nullptr;
    writeDescriptorSet.pBufferInfo = nullptr;
    writeDescriptorSet.pTexelBufferView = nullptr;
    updated = false;
    return writeDescriptorSet;
}

template<typename UniformBlockType>
inline InlineUniformBlock<UniformBlockType>& InlineUniformBlock<UniformBlockType>::operator=(const InlineUniformBlock<UniformBlockType>& other) noexcept
{
    if (this != &other)
    {
        binding = other.binding;
        descriptorType = other.descriptorType;
        descriptorCount = other.descriptorCount;
        stageFlags = other.stageFlags;
        pImmutableSamplers = other.pImmutableSampler;
        updated = other.updated;
        writeDescriptorSetInlineUniformBlock.sType = other.writeDescriptorSetInlineUniformBlock.sType;
        writeDescriptorSetInlineUniformBlock.pNext = other.writeDescriptorSetInlineUniformBlock.pNext;
        writeDescriptorSetInlineUniformBlock.dataSize = other.writeDescriptorSetInlineUniformBlock.dataSize;
        if (!writeDescriptorSetInlineUniformBlock.pData)
            writeDescriptorSetInlineUniformBlock.pData = new(std::nothrow) UniformBlockType();
        if (writeDescriptorSetInlineUniformBlock.pData)
            memcpy(writeDescriptorSetInlineUniformBlock.pData, other.writeDescriptorSetInlineUniformBlock.pData, sizeof(UniformBlockType));
    }
    return *this;
}

template<typename UniformBlockType>
inline InlineUniformBlock<UniformBlockType>& InlineUniformBlock<UniformBlockType>::operator=(const UniformBlockType& inlineUniformBlock) noexcept
{
    writeDescriptorSetInlineUniformBlock.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_INLINE_UNIFORM_BLOCK_EXT;
    writeDescriptorSetInlineUniformBlock.pNext = nullptr;
    writeDescriptorSetInlineUniformBlock.dataSize = sizeof(UniformBlockType);
    if (!writeDescriptorSetInlineUniformBlock.pData)
        writeDescriptorSetInlineUniformBlock.pData = new(std::nothrow) UniformBlockType();
    if (writeDescriptorSetInlineUniformBlock.pData)
        memcpy((void *)writeDescriptorSetInlineUniformBlock.pData, &inlineUniformBlock, sizeof(UniformBlockType));
    updated = true;
    return *this;
}
#endif // VK_EXT_inline_uniform_block
} // namespace binding
} // namespace magma
