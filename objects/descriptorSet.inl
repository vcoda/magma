namespace magma
{
#ifdef VK_EXT_inline_uniform_block
template<typename UniformBlockType>
void DescriptorSet::writeInlineUniformDescriptor(uint32_t index, const UniformBlockType& inlineUniformBlock)
{
    const DescriptorSetLayout::Binding& binding = layout->getBinding(index);
    MAGMA_ASSERT(VK_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK_EXT == binding.descriptorType);
    MAGMA_ASSERT(sizeof(UniformBlockType) == binding.descriptorCount); // Check size in bytes
    MAGMA_ASSERT(inlineUniformBlockDescriptors.capacity() - inlineUniformBlockDescriptors.size() >= 1);
    MAGMA_ASSERT(inlineUniformBlockAllocator->hasSpace(sizeof(UniformBlockType)));
    void *inlineUniformBlockData = inlineUniformBlockAllocator->alloc(sizeof(UniformBlockType), 16);
    if (inlineUniformBlockData)
        memcpy(inlineUniformBlockData, &inlineUniformBlock, sizeof(UniformBlockType));
    VkWriteDescriptorSetInlineUniformBlockEXT inlineUniformBlockDescriptorWrite;
    inlineUniformBlockDescriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET_INLINE_UNIFORM_BLOCK_EXT; 
    inlineUniformBlockDescriptorWrite.pNext = nullptr;
    inlineUniformBlockDescriptorWrite.dataSize = sizeof(UniformBlockType);
    inlineUniformBlockDescriptorWrite.pData = inlineUniformBlockData;
    inlineUniformBlockDescriptors.push_back(inlineUniformBlockDescriptorWrite);
    VkWriteDescriptorSet descriptorWrite;
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.pNext = &inlineUniformBlockDescriptors.back();
    descriptorWrite.dstSet = handle;
    descriptorWrite.dstBinding = binding.binding;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorCount = sizeof(UniformBlockType); // The <descriptorCount> provides the total number of bytes a particular binding can hold
    descriptorWrite.descriptorType = binding.descriptorType;
    descriptorWrite.pImageInfo = nullptr;
    descriptorWrite.pBufferInfo = nullptr;
    descriptorWrite.pTexelBufferView = nullptr;
    descriptorWrites.push_back(descriptorWrite);
}
#endif // VK_EXT_inline_uniform_block
} // namespace magma
