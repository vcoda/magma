namespace magma::helpers
{
template<class Type>
inline void mapScopedRange(const Buffer *buffer,
    VkDeviceSize offset, VkDeviceSize size,
    std::function<void(Type *data)> mapFn)
{
    MAGMA_ASSERT(buffer);
    MAGMA_ASSERT(mapFn);
    MAGMA_ASSERT(offset + (VK_WHOLE_SIZE == size ? 0 : size) <= buffer->getSize());
    auto& bufferMemory = buffer->getMemory();
    if (bufferMemory)
    {
        void *data = bufferMemory->map(offset, size);
        if (data) try
        {
            mapFn(reinterpret_cast<Type *>(data));
            bufferMemory->unmap();
        }
        catch (...)
        {
            bufferMemory->unmap();
            MAGMA_THROW;
        }
    }
}

template<class Type>
inline void mapScopedRange(const std::unique_ptr<Buffer>& buffer, VkDeviceSize offset, VkDeviceSize size, std::function<void(Type *data)> mapFn)
{
    mapScopedRange(buffer.get(), offset, size, std::move(mapFn));
}

template<class Type>
inline void mapScopedRange(const std::shared_ptr<Buffer>& buffer, VkDeviceSize offset, VkDeviceSize size, std::function<void(Type *data)> mapFn)
{
    mapScopedRange(buffer.get(), offset, size, std::move(mapFn));
}

template<class Type>
inline void mapScoped(const std::unique_ptr<Buffer>& buffer, std::function<void(Type *data)> mapFn)
{
    mapScopedRange(buffer.get(), 0, VK_WHOLE_SIZE, std::move(mapFn));
}

template<class Type>
inline void mapScoped(const std::shared_ptr<Buffer>& buffer, std::function<void(Type *data)> mapFn)
{
    mapScopedRange(buffer.get(), 0, VK_WHOLE_SIZE, std::move(mapFn));
}

template<class Type>
inline void mapScoped(std::shared_ptr<UniformBuffer<Type>> uniformBuffer,
    std::function<void(typename UniformBuffer<Type>::UniformType *data)> mapFn)
{
    MAGMA_ASSERT(uniformBuffer);
    MAGMA_ASSERT(mapFn);
    void *data = uniformBuffer->map();
    if (data) try
    {
        mapFn(reinterpret_cast<Type *>(data));
        uniformBuffer->unmap();
    }
    catch (...)
    {
        uniformBuffer->unmap();
        MAGMA_THROW;
    }
}

template<class Type>
inline void mapScoped(std::shared_ptr<UniformBuffer<Type>> uniformBuffer,
    std::function<void(UniformArray<Type>& array)> mapFn)
{
    MAGMA_ASSERT(uniformBuffer);
    MAGMA_ASSERT(mapFn);
    void *data = uniformBuffer->map();
    if (data) try
    {
        UniformArray<Type> array(data, uniformBuffer->getArraySize());
        mapFn(array);
        uniformBuffer->unmap();
    }
    catch (...)
    {
        uniformBuffer->unmap();
        MAGMA_THROW;
    }
}

template<class Type>
inline void mapScoped(std::shared_ptr<DynamicUniformBuffer<Type>> uniformBuffer,
    std::function<void(AlignedUniformArray<Type>& array)> mapFn)
{
    MAGMA_ASSERT(uniformBuffer);
    MAGMA_ASSERT(mapFn);
    void *data = uniformBuffer->map();
    if (data) try
    {
        AlignedUniformArray<Type> array(data,
            uniformBuffer->getArraySize(),
            uniformBuffer->getAlignment());
        mapFn(array);
        uniformBuffer->unmap();
    }
    catch (...)
    {
        uniformBuffer->unmap();
        MAGMA_THROW;
    }
}

template<class Type>
inline void mapScoped(std::shared_ptr<NonCoherentUniformBuffer<Type>> uniformBuffer,
    std::function<void(UniformArray<Type>& array)> mapFn)
{
    MAGMA_ASSERT(uniformBuffer);
    MAGMA_ASSERT(mapFn);
    auto& memory = uniformBuffer->getMemory();
    const uint32_t arraySize = uniformBuffer->getArraySize();
    const VkDeviceSize alignment = uniformBuffer->getAlignment();
    VkDeviceSize offset = 0, size = 0;
    if (uniformBuffer->mappedPersistently())
    {
        UniformArray<Type> array(memory->getMapPointer(), arraySize, alignment);
        mapFn(array);
        offset = array.getFirstIndex() * alignment;
        size = array.getUpdatedRange() * alignment;
    }
    else
    {
        void *data = uniformBuffer->map();
        if (data) try
        {
            UniformArray<Type> array(data, arraySize, alignment);
            mapFn(array);
            offset = array.getFirstIndex() * alignment;
            size = array.getUpdatedRange() * alignment;
        }
        catch (...)
        {
            uniformBuffer->unmap();
            MAGMA_THROW;
        }
    }
    if (size)
    {
        const VkDeviceSize nonCoherentAtomSize = uniformBuffer->getNonCoherentAtomSize();
        if (offset % nonCoherentAtomSize)
            offset = offset / nonCoherentAtomSize * nonCoherentAtomSize;
        const VkDeviceSize minFlushSize = std::min(memory->getSize(), nonCoherentAtomSize);
        size = std::max(size, minFlushSize);
        // vkFlushMappedMemoryRanges() guarantees that host writes to the
        // memory ranges are made available to the host memory domain,
        // such that they can be made available to the device memory domain
        // using the VK_ACCESS_HOST_WRITE_BIT access type.
        // Unmapping non-coherent memory does not implicitly flush the host
        // mapped memory, and host writes that have not been flushed may not
        // ever be visible to the device.
        memory->flushMappedRange(offset, size);
    }
    uniformBuffer->unmap();
}

template<class Type>
inline void mapScoped(std::shared_ptr<NonCoherentDynamicUniformBuffer<Type>> uniformBuffer,
    std::function<void(AlignedUniformArray<Type>& array)> mapFn)
{
    MAGMA_ASSERT(uniformBuffer);
    MAGMA_ASSERT(mapFn);
    auto& memory = uniformBuffer->getMemory();
    const uint32_t arraySize = uniformBuffer->getArraySize();
    const VkDeviceSize alignment = uniformBuffer->getAlignment();
    VkDeviceSize offset = 0, size = 0;
    if (uniformBuffer->mappedPersistently())
    {
        AlignedUniformArray<Type> array(memory->getMapPointer(), arraySize, alignment);
        mapFn(array);
        offset = array.getFirstIndex() * alignment;
        size = array.getUpdatedRange() * alignment;
    }
    else
    {
        void *data = uniformBuffer->map();
        if (data) try
        {
            AlignedUniformArray<Type> array(data, arraySize, alignment);
            mapFn(array);
            offset = array.getFirstIndex() * alignment;
            size = array.getUpdatedRange() * alignment;
        }
        catch (...)
        {
            uniformBuffer->unmap();
            MAGMA_THROW;
        }
    }
    if (size)
    {
        const VkDeviceSize nonCoherentAtomSize = uniformBuffer->getNonCoherentAtomSize();
        if (offset % nonCoherentAtomSize)
            offset = offset / nonCoherentAtomSize * nonCoherentAtomSize;
        const VkDeviceSize minFlushSize = std::min(memory->getSize(), nonCoherentAtomSize);
        size = std::max(size, minFlushSize);
        memory->flushMappedRange(offset, size);
    }
    uniformBuffer->unmap();
}

template<class Type>
inline void mapScopedRange(std::shared_ptr<Image> image,
    VkDeviceSize offset,
    VkDeviceSize size,
    std::function<void(Type *data)> mapFn)
{
    MAGMA_ASSERT(image);
    MAGMA_ASSERT(mapFn);
    MAGMA_ASSERT(offset + (VK_WHOLE_SIZE == size ? 0 : size) <= image->getSize());
    auto& imageMemory = image->getMemory();
    if (imageMemory)
    {
        void *data = imageMemory->map(offset, size);
        if (data) try
        {
            mapFn(static_cast<Type *>(data));
            imageMemory->unmap();
        }
        catch (...)
        {
            imageMemory->unmap();
            MAGMA_THROW;
        }
    }
}

template<class Type>
inline void mapScoped(std::shared_ptr<Image> image,
    std::function<void(Type *data)> mapFn)
{
    mapScopedRange(std::move(image), 0, VK_WHOLE_SIZE, std::move(mapFn));
}
} // namespace magma::helpers