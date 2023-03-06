namespace magma
{
namespace descriptor
{
inline Descriptor::Descriptor(VkDescriptorType descriptorType, uint32_t descriptorCount, uint32_t binding_) noexcept:
    imageType(VK_IMAGE_TYPE_MAX_ENUM),
    updated(false)
{
    binding.binding = binding_;
    binding.descriptorType = descriptorType;
    binding.descriptorCount = descriptorCount;
    binding.stageFlags = 0;
    binding.pImmutableSamplers = nullptr;
}

inline void Descriptor::updateSampler(std::shared_ptr<const magma::Sampler> sampler) noexcept
{
    const VkSampler handle = MAGMA_OPTIONAL_HANDLE(sampler);
    if (imageDescriptor.sampler != handle)
    {
        imageDescriptor.sampler = handle;
        imageDescriptor.imageView = VK_NULL_HANDLE;
        imageDescriptor.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        updated = true;
    }
}

inline void Descriptor::updateImageView(std::shared_ptr<const ImageView> imageView, VkImageUsageFlags requiredUsage) noexcept
{
    MAGMA_UNUSED(requiredUsage);
    MAGMA_ASSERT(imageView);
    MAGMA_ASSERT(imageView->getImage()->getUsage() & requiredUsage);
    if (imageDescriptor.imageView != *imageView)
    {
        imageDescriptor = imageView->getDescriptor(nullptr);
        imageType = imageView->getImage()->getType();
        updated = true;
    }
}

inline void Descriptor::updateBufferView(std::shared_ptr<const BufferView> bufferView, VkBufferUsageFlags requiredUsage) noexcept
{
    MAGMA_UNUSED(requiredUsage);
    MAGMA_ASSERT(bufferView);
    MAGMA_ASSERT(bufferView->getBuffer()->getUsage() & requiredUsage);
    if (texelBufferView != *bufferView)
    {
        texelBufferView = *bufferView;
        updated = true;
    }
}

inline void Descriptor::updateBuffer(std::shared_ptr<const Buffer> buffer, VkBufferUsageFlags requiredUsage) noexcept
{
    MAGMA_UNUSED(requiredUsage);
    MAGMA_ASSERT(buffer);
    MAGMA_ASSERT(buffer->getUsage() & requiredUsage);
    if (bufferDescriptor.buffer != *buffer)
    {   // TODO: offset, range?
        bufferDescriptor = buffer->getDescriptor();
        updated = true;
    }
}

inline Sampler& Sampler::operator=(std::shared_ptr<const magma::Sampler> sampler) noexcept
{
    MAGMA_ASSERT(sampler);
    updateSampler(std::move(sampler));
    return *this;
}

inline ImmutableSampler& ImmutableSampler::operator=(std::shared_ptr<const magma::Sampler> sampler) noexcept
{   // Immutable sampler must be updated only once
    MAGMA_ASSERT(!binding.pImmutableSamplers);
    if (!binding.pImmutableSamplers)
    {
        updateSampler(nullptr);
        // If pImmutableSamplers is not NULL, then it is a pointer to an array of sampler handles
        // that will be copied into the set layout and used for the corresponding binding.
        MAGMA_ASSERT(sampler);
        binding.pImmutableSamplers = sampler->getHandleAddress();
        updated = true;
    }
    return *this;
}

inline CombinedImageSampler& CombinedImageSampler::operator=(const std::pair<std::shared_ptr<const ImageView>, std::shared_ptr<const magma::Sampler>>& combinedImageSampler) noexcept
{
    MAGMA_ASSERT(combinedImageSampler.first);
    MAGMA_ASSERT(combinedImageSampler.second);
    MAGMA_ASSERT(combinedImageSampler.first->getImage()->getUsage() & VK_IMAGE_USAGE_SAMPLED_BIT);
    if ((imageDescriptor.imageView != *combinedImageSampler.first) ||
        (imageDescriptor.sampler != *combinedImageSampler.second))
    {
        imageDescriptor = combinedImageSampler.first->getDescriptor(combinedImageSampler.second);
        updated = true;
    }
    return *this;
}

inline CombinedImageImmutableSampler& CombinedImageImmutableSampler::operator=(const std::pair<std::shared_ptr<const ImageView>, std::shared_ptr<const magma::Sampler>>& combinedImageSampler) noexcept
{
    updateImageView(combinedImageSampler.first, VK_IMAGE_USAGE_SAMPLED_BIT);
    // Immutable sampler must be updated only once
    if (!binding.pImmutableSamplers)
    {   // If pImmutableSamplers is not NULL, then it is a pointer to an array of sampler handles
        // that will be copied into the set layout and used for the corresponding binding.
        MAGMA_ASSERT(combinedImageSampler.second);
        binding.pImmutableSamplers = combinedImageSampler.second->getHandleAddress();
        updated = true;
    }
    return *this;
}

inline CombinedImageImmutableSampler& CombinedImageImmutableSampler::operator=(std::shared_ptr<const ImageView> imageView) noexcept
{   // Check that sampler is already set and stop carrying around it
    MAGMA_ASSERT(binding.pImmutableSamplers);
    updateImageView(std::move(imageView), VK_IMAGE_USAGE_SAMPLED_BIT);
    return *this;
}

inline SampledImage& SampledImage::operator=(std::shared_ptr<const ImageView> imageView) noexcept
{
    updateImageView(std::move(imageView), VK_IMAGE_USAGE_SAMPLED_BIT);
    return *this;
}

inline StorageImage& StorageImage::operator=(std::shared_ptr<const ImageView> imageView) noexcept
{
    updateImageView(std::move(imageView), VK_IMAGE_USAGE_STORAGE_BIT);
    return *this;
}

inline UniformTexelBuffer& UniformTexelBuffer::operator=(std::shared_ptr<const BufferView> bufferView) noexcept
{
    updateBufferView(std::move(bufferView), VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT);
    return *this;
}

inline StorageTexelBuffer& StorageTexelBuffer::operator=(std::shared_ptr<const BufferView> bufferView) noexcept
{
    updateBufferView(std::move(bufferView), VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT);
    return *this;
}

inline UniformBuffer& UniformBuffer::operator=(std::shared_ptr<const Buffer> buffer) noexcept
{
    updateBuffer(std::move(buffer), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
    return *this;
}

inline StorageBuffer& StorageBuffer::operator=(std::shared_ptr<const Buffer> buffer) noexcept
{
    updateBuffer(std::move(buffer), VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
    return *this;
}

inline DynamicUniformBuffer& DynamicUniformBuffer::operator=(std::shared_ptr<const Buffer> buffer) noexcept
{
    updateBuffer(std::move(buffer), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
    return *this;
}

inline DynamicStorageBuffer& DynamicStorageBuffer::operator=(std::shared_ptr<const Buffer> buffer) noexcept
{
    updateBuffer(std::move(buffer), VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
    return *this;
}

inline InputAttachment& InputAttachment::operator=(std::shared_ptr<const ImageView> imageView) noexcept
{
    updateImageView(std::move(imageView), VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT);
    return *this;
}
} // namespace descriptor
} // namespace magma
