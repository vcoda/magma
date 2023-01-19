namespace magma
{
namespace descriptor
{
inline Descriptor::Descriptor(VkDescriptorType descriptorType, uint32_t descriptorCount, uint32_t binding_) noexcept:
    updated(false)
{
    binding.binding = binding_;
    binding.descriptorType = descriptorType;
    binding.descriptorCount = descriptorCount;
    binding.stageFlags = 0;
    binding.pImmutableSamplers = nullptr;
}

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
    MAGMA_ASSERT(!binding.pImmutableSamplers);
    // Immutable sampler must be updated only once
    if (!binding.pImmutableSamplers)
    {
        imageDescriptor.sampler = VK_NULL_HANDLE;
        imageDescriptor.imageView = VK_NULL_HANDLE;
        imageDescriptor.imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        // If pImmutableSamplers is not NULL, then it is a pointer to an array of sampler handles
        // that will be copied into the set layout and used for the corresponding binding.
        binding.pImmutableSamplers = sampler->getImmutableSampler();
        updated = true;
    }
    return *this;
}

inline CombinedImageSampler& CombinedImageSampler::operator=(const std::pair<std::shared_ptr<const ImageView>, std::shared_ptr<const magma::Sampler>>& combinedImageSampler) noexcept
{
    MAGMA_ASSERT(combinedImageSampler.first);
    MAGMA_ASSERT(combinedImageSampler.second);
    MAGMA_ASSERT(combinedImageSampler.first->getImage()->getUsage() & VK_IMAGE_USAGE_SAMPLED_BIT);
    imageDescriptor = combinedImageSampler.first->getDescriptor(combinedImageSampler.second);
    updated = true;
    return *this;
}

inline CombinedImageImmutableSampler& CombinedImageImmutableSampler::operator=(const std::pair<std::shared_ptr<const ImageView>, std::shared_ptr<const magma::Sampler>>& combinedImageSampler) noexcept
{
    MAGMA_ASSERT(combinedImageSampler.first);
    MAGMA_ASSERT(combinedImageSampler.second);
    MAGMA_ASSERT(combinedImageSampler.first->getImage()->getUsage() & VK_IMAGE_USAGE_SAMPLED_BIT);
    imageDescriptor = combinedImageSampler.first->getDescriptor(nullptr);
    // Immutable sampler must be updated only once
    if (!binding.pImmutableSamplers)
    {   // If pImmutableSamplers is not NULL, then it is a pointer to an array of sampler handles
        // that will be copied into the set layout and used for the corresponding binding.
        binding.pImmutableSamplers = combinedImageSampler.second->getImmutableSampler();
    }
    updated = true;
    return *this;
}

inline CombinedImageImmutableSampler& CombinedImageImmutableSampler::operator=(std::shared_ptr<const ImageView> imageView) noexcept
{
    MAGMA_ASSERT(imageView);
    MAGMA_ASSERT(imageView->getImage()->getUsage() & VK_IMAGE_USAGE_SAMPLED_BIT);
    MAGMA_ASSERT(binding.pImmutableSamplers); // Check that sampler is already set and stop carrying around it
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
} // namespace descriptor
} // namespace magma
