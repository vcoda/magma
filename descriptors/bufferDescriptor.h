/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2023 Victor Coda.

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
#include "descriptor.h"

namespace magma
{
    namespace descriptor
    {
        /* A uniform texel buffer is a descriptor type associated with a buffer resource
           via a buffer view that formatted load operations can be performed on. */

        class UniformTexelBuffer : public Descriptor
        {
        public:
            UniformTexelBuffer(uint32_t binding) noexcept:
                Descriptor(VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1, binding) {}
            UniformTexelBuffer& operator=(std::shared_ptr<const BufferView>) noexcept;
        };

        /* A storage texel buffer is a descriptor type associated with a buffer resource
           via a buffer view that formatted load, store, and atomic operations can be performed on. */

        class StorageTexelBuffer : public Descriptor
        {
        public:
            StorageTexelBuffer(uint32_t binding) noexcept:
                Descriptor(VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1, binding) {}
            StorageTexelBuffer& operator=(std::shared_ptr<const BufferView>) noexcept;
        };

        /* A uniform buffer is a descriptor type associated with a buffer resource directly,
           described in a shader as a structure with various members that load operations can be performed on. */

        class UniformBuffer : public Descriptor
        {
        public:
            UniformBuffer(uint32_t binding) noexcept:
                Descriptor(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, binding) {}
            UniformBuffer& operator=(std::shared_ptr<const Buffer>) noexcept;
        };

        /* A storage buffer is a descriptor type associated with a buffer resource directly,
           described in a shader as a structure with various members that load, store,
           and atomic operations can be performed on. */

        class StorageBuffer : public Descriptor
        {
        public:
            StorageBuffer(uint32_t binding) noexcept:
                Descriptor(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, binding) {}
            StorageBuffer& operator=(std::shared_ptr<const Buffer>) noexcept;
        };

        /* A dynamic uniform buffer is almost identical to a uniform buffer,
           and differs only in how the offset into the buffer is specified.
           The base offset calculated by the VkDescriptorBufferInfo when
           initially updating the descriptor set is added to a dynamic offset
           when binding the descriptor set. */

        class DynamicUniformBuffer : public Descriptor
        {
        public:
            DynamicUniformBuffer(uint32_t binding) noexcept:
                Descriptor(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1, binding) {}
            DynamicUniformBuffer& operator=(std::shared_ptr<const Buffer>) noexcept;
        };

        /* A dynamic storage buffer is almost identical to a storage buffer,
           and differs only in how the offset into the buffer is specified.
           The base offset calculated by the VkDescriptorBufferInfo when
           initially updating the descriptor set is added to a dynamic offset
           when binding the descriptor set. */

        class DynamicStorageBuffer : public Descriptor
        {
        public:
            DynamicStorageBuffer(uint32_t binding) noexcept:
                Descriptor(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1, binding) {}
            DynamicStorageBuffer& operator=(std::shared_ptr<const Buffer>) noexcept;
        };
    } // namespace descriptor
} // namespace magma

#include "bufferDescriptor.inl"