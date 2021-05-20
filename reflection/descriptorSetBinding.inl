namespace magma
{
namespace reflection
{
#ifdef VK_EXT_inline_uniform_block
template<typename UniformBlockType>
inline InlineUniformBlock<UniformBlockType>::InlineUniformBlock(uint32_t binding /* 0 */) noexcept:
    DescriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK_EXT, sizeof(UniformBlockType), binding),
    inlineUniformBlockDescriptorWrite{}
{}

template<typename UniformBlockType>
inline InlineUniformBlock<UniformBlockType>::~InlineUniformBlock()
{
    delete (UniformBlockType *)inlineUniformBlockDescriptorWrite.pData;
}

template<typename UniformBlockType>
inline InlineUniformBlock<UniformBlockType>& InlineUniformBlock<UniformBlockType>::operator=(const UniformBlockType& inlineUniformBlock) noexcept
{
    inlineUniformBlockDescriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_INLINE_UNIFORM_BLOCK_EXT; 
    inlineUniformBlockDescriptorWrite.pNext = nullptr;
    inlineUniformBlockDescriptorWrite.dataSize = sizeof(UniformBlockType);
    inlineUniformBlockDescriptorWrite.pData = new(std::nothrow) UniformBlockType();
    if (inlineUniformBlockDescriptorWrite.pData)
        memcpy((void *)inlineUniformBlockDescriptorWrite.pData, &inlineUniformBlock, sizeof(UniformBlockType));
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.pNext = &inlineUniformBlockDescriptorWrite;
    descriptorWrite.dstSet = VK_NULL_HANDLE;
    descriptorWrite.dstBinding = binding;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorCount = sizeof(UniformBlockType); // The <descriptorCount> provides the total number of bytes a particular binding can hold
    descriptorWrite.descriptorType = descriptorType;
    descriptorWrite.pImageInfo = nullptr;
    descriptorWrite.pBufferInfo = nullptr;
    descriptorWrite.pTexelBufferView = nullptr;
    updated = true;
    return *this;
}
#endif // VK_EXT_inline_uniform_block
} // namespace reflection
} // namespace magma
