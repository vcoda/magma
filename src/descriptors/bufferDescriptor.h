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
#include "descriptorSetLayoutBinding.h"

namespace magma
{
    class Buffer;
    class BufferView;
}

namespace magma::descriptor
{
    /* Base class of buffer descriptor. */

    class BufferDescriptor : public magma::DescriptorSetLayoutBinding
    {
    public:
        bool resourceBinded() const noexcept override;
        void write(VkDescriptorSet dstSet,
            VkWriteDescriptorSet& writeDescriptorSet) const noexcept override;

    protected:
        BufferDescriptor(VkDescriptorType descriptorType,
            uint32_t binding) noexcept;
        void update(lent_ptr<const Buffer> buffer,
            VkBufferUsageFlags usage) noexcept;

    private:
        VkDescriptorBufferInfo descriptor;
    };

    /* Base class of texel buffer descriptor. */

    class TexelBufferDescriptor : public magma::DescriptorSetLayoutBinding
    {
    public:
        bool resourceBinded() const noexcept override;
        void write(VkDescriptorSet dstSet,
            VkWriteDescriptorSet& writeDescriptorSet) const noexcept override;

    protected:
        TexelBufferDescriptor(VkDescriptorType descriptorType,
            uint32_t binding) noexcept;
        void update(lent_ptr<const BufferView> bufferView,
            VkBufferUsageFlags usage) noexcept;

    private:
        VkBufferView descriptor;
    };

    /* A uniform texel buffer is a descriptor type associated with
       a buffer resource via a buffer view that formatted load
       operations can be performed on. */

    class UniformTexelBuffer : public TexelBufferDescriptor
    {
    public:
        UniformTexelBuffer(uint32_t binding) noexcept:
            TexelBufferDescriptor(VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, binding) {}
        UniformTexelBuffer& operator=(lent_ptr<const BufferView>) noexcept;
    };

    /* A storage texel buffer is a descriptor type associated with
       a buffer resource via a buffer view that formatted load,
       store, and atomic operations can be performed on. */

    class StorageTexelBuffer : public TexelBufferDescriptor
    {
    public:
        StorageTexelBuffer(uint32_t binding) noexcept:
            TexelBufferDescriptor(VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, binding) {}
        StorageTexelBuffer& operator=(lent_ptr<const BufferView>) noexcept;
    };

    /* A uniform buffer is a descriptor type associated with
       a buffer resource directly, described in a shader as
       a structure with various members that load operations
       can be performed on. */

    class UniformBuffer : public BufferDescriptor
    {
    public:
        UniformBuffer(uint32_t binding) noexcept:
            BufferDescriptor(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, binding) {}
        UniformBuffer& operator=(lent_ptr<const Buffer>) noexcept;
    };

    /* A storage buffer is a descriptor type associated with
       a buffer resource directly, described in a shader as
       a structure with various members that load, store, and
       atomic operations can be performed on. */

    class StorageBuffer : public BufferDescriptor
    {
    public:
        StorageBuffer(uint32_t binding) noexcept:
            BufferDescriptor(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, binding) {}
        StorageBuffer& operator=(lent_ptr<const Buffer>) noexcept;
    };

    /* A dynamic uniform buffer is almost identical to a uniform buffer,
       and differs only in how the offset into the buffer is specified.
       The base offset calculated by the VkDescriptorBufferInfo when
       initially updating the descriptor set is added to a dynamic
       offset when binding the descriptor set. */

    class DynamicUniformBuffer : public BufferDescriptor
    {
    public:
        DynamicUniformBuffer(uint32_t binding) noexcept:
            BufferDescriptor(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, binding) {}
        DynamicUniformBuffer& operator=(lent_ptr<const Buffer>) noexcept;
    };

    /* A dynamic storage buffer is almost identical to a storage buffer,
       and differs only in how the offset into the buffer is specified.
       The base offset calculated by the VkDescriptorBufferInfo when
       initially updating the descriptor set is added to a dynamic
       offset when binding the descriptor set. */

    class DynamicStorageBuffer : public BufferDescriptor
    {
    public:
        DynamicStorageBuffer(uint32_t binding) noexcept:
            BufferDescriptor(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, binding) {}
        DynamicStorageBuffer& operator=(lent_ptr<const Buffer>) noexcept;
    };
} // namespace magma::descriptor

#include "bufferDescriptor.inl"
