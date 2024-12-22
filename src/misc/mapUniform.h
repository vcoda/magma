/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2024 Victor Coda.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://www.gnu.org/licenses/>.
*/
#pragma once
#include "../objects/uniformBuffer.h"
#include "../objects/dynamicUniformBuffer.h"

namespace magma
{
template<class Type>
inline void map(UniformBuffer<Type> *uniformBuffer,
    std::function<void(typename UniformBuffer<Type>::UniformType *data)> mapFn)
{
    MAGMA_ASSERT(uniformBuffer);
    MAGMA_ASSERT(sizeof(Type) <= uniformBuffer->getSize());
    void *data = uniformBuffer->map();
    MAGMA_ASSERT(data);
    if (data) try
    {
        MAGMA_ASSERT(mapFn);
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
inline void map(const std::unique_ptr<UniformBuffer<Type>>& uniformBuffer,
    std::function<void(typename UniformBuffer<Type>::UniformType *data)> mapFn)
{
    map(uniformBuffer.get(), std::move(mapFn));
}

template<class Type>
inline void map(const std::shared_ptr<UniformBuffer<Type>>& uniformBuffer,
    std::function<void(typename UniformBuffer<Type>::UniformType *data)> mapFn)
{
    map(uniformBuffer.get(), std::move(mapFn));
}

template<class Type>
inline void map(DynamicUniformBuffer<Type> *uniformBuffer,
    std::function<void(helpers::AlignedUniformArray<Type>& array)> mapFn)
{
    MAGMA_ASSERT(uniformBuffer);
    MAGMA_ASSERT(sizeof(Type) <= uniformBuffer->getSize());
    void *data = uniformBuffer->map();
    MAGMA_ASSERT(data);
    if (data) try
    {
        helpers::AlignedUniformArray<Type> array(data,
            uniformBuffer->getArraySize(),
            uniformBuffer->getAlignment());
        MAGMA_ASSERT(mapFn);
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
inline void map(const std::unique_ptr<DynamicUniformBuffer<Type>>& uniformBuffer,
    std::function<void(helpers::AlignedUniformArray<Type>& array)> mapFn)
{
    return map(uniformBuffer.get(), std::move(mapFn));
}

template<class Type>
inline void map(const std::shared_ptr<DynamicUniformBuffer<Type>>& uniformBuffer,
    std::function<void(helpers::AlignedUniformArray<Type>& array)> mapFn)
{
    return map(uniformBuffer.get(), std::move(mapFn));
}

template<class Type>
inline void map(NonCoherentUniformBuffer<Type> *uniformBuffer,
    std::function<void(helpers::UniformArray<Type>& array)> mapFn)
{
    MAGMA_ASSERT(uniformBuffer);
    MAGMA_ASSERT(sizeof(Type) <= uniformBuffer->getSize());
    auto& memory = uniformBuffer->getMemory();
    const uint32_t arraySize = uniformBuffer->getArraySize();
    const VkDeviceSize alignment = uniformBuffer->getAlignment();
    VkDeviceSize offset = 0, size = 0;
    if (uniformBuffer->mappedPersistently())
    {
        helpers::UniformArray<Type> array(memory->getMapPointer(), arraySize, alignment);
        mapFn(array);
        offset = array.getFirstIndex() * alignment;
        size = array.getUpdatedRange() * alignment;
    }
    else
    {
        void *data = uniformBuffer->map();
        MAGMA_ASSERT(data);
        if (data) try
        {
            helpers::UniformArray<Type> array(data, arraySize, alignment);
            MAGMA_ASSERT(mapFn);
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
    if (size && !memory->getFlags().hostCoherent)
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
inline void map(const std::unique_ptr<NonCoherentUniformBuffer<Type>>& uniformBuffer,
    std::function<void(helpers::UniformArray<Type>& array)> mapFn)
{
    map(uniformBuffer.get(), std::move(mapFn));
}

template<class Type>
inline void map(const std::shared_ptr<NonCoherentUniformBuffer<Type>>& uniformBuffer,
    std::function<void(helpers::UniformArray<Type>& array)> mapFn)
{
    map(uniformBuffer.get(), std::move(mapFn));
}

template<class Type>
inline void map(NonCoherentDynamicUniformBuffer<Type> *uniformBuffer,
    std::function<void(helpers::AlignedUniformArray<Type>& array)> mapFn)
{
    MAGMA_ASSERT(uniformBuffer);
    MAGMA_ASSERT(sizeof(Type) <= uniformBuffer->getSize());
    auto& memory = uniformBuffer->getMemory();
    const uint32_t arraySize = uniformBuffer->getArraySize();
    const VkDeviceSize alignment = uniformBuffer->getAlignment();
    VkDeviceSize offset = 0, size = 0;
    if (uniformBuffer->mappedPersistently())
    {
        helpers::AlignedUniformArray<Type> array(memory->getMapPointer(), arraySize, alignment);
        MAGMA_ASSERT(mapFn);
        mapFn(array);
        offset = array.getFirstIndex() * alignment;
        size = array.getUpdatedRange() * alignment;
    }
    else
    {
        void *data = uniformBuffer->map();
        MAGMA_ASSERT(data);
        if (data) try
        {
            helpers::AlignedUniformArray<Type> array(data, arraySize, alignment);
            MAGMA_ASSERT(mapFn);
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
    if (size && !memory->getFlags().hostCoherent)
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
inline void map(const std::unique_ptr<NonCoherentDynamicUniformBuffer<Type>>& uniformBuffer,
    std::function<void(helpers::AlignedUniformArray<Type>& array)> mapFn)
{
    map(uniformBuffer.get(), std::move(mapFn));
}

template<class Type>
inline void map(const std::shared_ptr<NonCoherentDynamicUniformBuffer<Type>>& uniformBuffer,
    std::function<void(helpers::AlignedUniformArray<Type>& array)> mapFn)
{
    map(uniformBuffer.get(), std::move(mapFn));
}
} // namespace magma
