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
    if (inlineUniformBlocks.empty())
    {   // Allocate placement buffer on first use
        inlineUniformBlocksSpace = MAGMA_MAX_INLINE_UNIFORM_BLOCK_BUFFER_SIZE;
        inlineUniformBlocks.resize(inlineUniformBlocksSpace);
        std::fill(inlineUniformBlocks.begin(), inlineUniformBlocks.end(), '\0');
        inlineUniformBlocksHead = inlineUniformBlocks.data();
    }
    MAGMA_ASSERT(inlineUniformBlocksSpace > 0);
    void *inlineUniformBlockData = nullptr;
    if (inlineUniformBlockData = std::align(16, sizeof(UniformBlockType), inlineUniformBlocksHead, inlineUniformBlocksSpace))
    {   // https://en.cppreference.com/w/cpp/memory/align
        memcpy(inlineUniformBlockData, &inlineUniformBlock, sizeof(UniformBlockType));
        inlineUniformBlocksHead = (char *)inlineUniformBlocksHead + sizeof(UniformBlockType);
        inlineUniformBlocksSpace -= sizeof(UniformBlockType);
    } 
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
    descriptorWrite.descriptorCount = binding.descriptorCount; // The <descriptorCount> provides the total number of bytes a particular binding can hold
    descriptorWrite.descriptorType = binding.descriptorType;
    descriptorWrite.pImageInfo = nullptr;
    descriptorWrite.pBufferInfo = nullptr;
    descriptorWrite.pTexelBufferView = nullptr;
    descriptorWrites.push_back(descriptorWrite);
}
#endif // VK_EXT_inline_uniform_block
} // namespace magma
