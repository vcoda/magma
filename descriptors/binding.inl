namespace magma
{
namespace binding
{
inline Sampler& Sampler::operator=(std::shared_ptr<const magma::Sampler> sampler) noexcept
{
	MAGMA_ASSERT(sampler);
    imageDescriptor.sampler = *sampler;
    imageDescriptor.imageView = VK_NULL_HANDLE;
    imageDescriptor.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    updated = true;
    return *this;
}

inline ImmutableSampler& ImmutableSampler::operator=(std::shared_ptr<const magma::Sampler> sampler) noexcept
{
	MAGMA_ASSERT(sampler);
    MAGMA_ASSERT(!pImmutableSamplers);
	// Immutable sampler must be updated only once
    if (!pImmutableSamplers)
    {
        imageDescriptor.sampler = VK_NULL_HANDLE;
        imageDescriptor.imageView = VK_NULL_HANDLE;
        imageDescriptor.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        // If pImmutableSamplers is not NULL, then it is a pointer to an array of sampler handles
        // that will be copied into the set layout and used for the corresponding binding.
        pImmutableSamplers = sampler->getImmutableSampler();
		updated = true;
    }
    return *this;
}

inline CombinedImageSampler& CombinedImageSampler::operator=(const std::pair<std::shared_ptr<const ImageView>, std::shared_ptr<const magma::Sampler>>& combinedImageSampler) noexcept
{
	MAGMA_ASSERT(combinedImageSampler.first);
	MAGMA_ASSERT(combinedImageSampler.second);
    imageDescriptor = combinedImageSampler.first->getDescriptor(combinedImageSampler.second);
	updated = true;
    return *this;
}

inline CombinedImageImmutableSampler& CombinedImageImmutableSampler::operator=(const std::pair<std::shared_ptr<const ImageView>, std::shared_ptr<const magma::Sampler>>& combinedImageSampler) noexcept
{
	MAGMA_ASSERT(combinedImageSampler.first);
	MAGMA_ASSERT(combinedImageSampler.second);
    imageDescriptor = combinedImageSampler.first->getDescriptor(nullptr);
	// Immutable sampler must be updated only once
    if (!pImmutableSamplers)
    {	// If pImmutableSamplers is not NULL, then it is a pointer to an array of sampler handles
        // that will be copied into the set layout and used for the corresponding binding.
        pImmutableSamplers = combinedImageSampler.second->getImmutableSampler();
    }
	updated = true;
    return *this;
}

inline CombinedImageImmutableSampler& CombinedImageImmutableSampler::operator=(std::shared_ptr<const ImageView> imageView) noexcept
{
	MAGMA_ASSERT(imageView);
	MAGMA_ASSERT(pImmutableSamplers); // Check that sampler is already set and stop carrying around it
    imageDescriptor = imageView->getDescriptor(nullptr);
	updated = true;
    return *this;
}

inline SampledImage& SampledImage::operator=(std::shared_ptr<const ImageView> imageView) noexcept
{
	MAGMA_ASSERT(imageView);
    MAGMA_ASSERT(imageView->getImage()->getUsage() & VK_IMAGE_USAGE_SAMPLED_BIT);
    imageDescriptor = imageView->getDescriptor(nullptr);
	updated = true;
    return *this;
}

inline StorageImage& StorageImage::operator=(std::shared_ptr<const ImageView> imageView) noexcept
{
	MAGMA_ASSERT(imageView);
    MAGMA_ASSERT(imageView->getImage()->getUsage() & VK_IMAGE_USAGE_STORAGE_BIT);
    imageDescriptor = imageView->getDescriptor(nullptr);
	updated = true;
    return *this;
}

inline UniformTexelBuffer& UniformTexelBuffer::operator=(std::shared_ptr<const BufferView> bufferView) noexcept
{
	MAGMA_ASSERT(bufferView);
    MAGMA_ASSERT(bufferView->getBuffer()->getUsage() & VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT);
    texelBufferView = *bufferView;
	updated = true;
    return *this;
}

inline StorageTexelBuffer& StorageTexelBuffer::operator=(std::shared_ptr<const BufferView> bufferView) noexcept
{
	MAGMA_ASSERT(bufferView);
    MAGMA_ASSERT(bufferView->getBuffer()->getUsage() & VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT);
    texelBufferView = *bufferView;
	updated = true;
    return *this;
}

inline UniformBuffer& UniformBuffer::operator=(std::shared_ptr<const Buffer> buffer) noexcept
{
	MAGMA_ASSERT(buffer);
    MAGMA_ASSERT(buffer->getUsage() & VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
    bufferDescriptor = buffer->getDescriptor();
	updated = true;
    return *this;
}

inline StorageBuffer& StorageBuffer::operator=(std::shared_ptr<const Buffer> buffer) noexcept
{
	MAGMA_ASSERT(buffer);
    MAGMA_ASSERT(buffer->getUsage() & VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
    bufferDescriptor = buffer->getDescriptor();
	updated = true;
    return *this;
}

inline DynamicUniformBuffer& DynamicUniformBuffer::operator=(std::shared_ptr<const Buffer> buffer) noexcept
{
	MAGMA_ASSERT(buffer);
    MAGMA_ASSERT(buffer->getUsage() & VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
    bufferDescriptor = buffer->getDescriptor();
	updated = true;
    return *this;
}

inline DynamicStorageBuffer& DynamicStorageBuffer::operator=(std::shared_ptr<const Buffer> buffer) noexcept
{
	MAGMA_ASSERT(buffer);
    MAGMA_ASSERT(buffer->getUsage() & VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
    bufferDescriptor = buffer->getDescriptor();
	updated = true;
    return *this;
}

inline InputAttachment& InputAttachment::operator=(std::shared_ptr<const ImageView> imageView) noexcept
{
	MAGMA_ASSERT(imageView);
    MAGMA_ASSERT(imageView->getImage()->getUsage() & VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT);
    imageDescriptor = imageView->getDescriptor(nullptr);
	updated = true;
    return *this;
}

#ifdef VK_EXT_inline_uniform_block
template<typename UniformBlockType>
inline InlineUniformBlock<UniformBlockType>::InlineUniformBlock(uint32_t binding) noexcept:
    DescriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK_EXT, sizeof(UniformBlockType), binding)
{}

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
