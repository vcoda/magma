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
#include "descriptorArray.h"

namespace magma
{
    namespace descriptor
    {
        /* A uniform texel buffer is a descriptor type associated with a buffer resource
           via a buffer view that formatted load operations can be performed on. */

        template<uint32_t Size>
        class UniformTexelBufferArray : public TDescriptorArray<Size>
        {
        public:
            UniformTexelBufferArray(uint32_t binding) noexcept:
                TDescriptorArray<Size>(VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, binding) {}
            DescriptorArray::BufferDescriptor operator[](uint32_t index) noexcept;
        };

        /* A storage texel buffer is a descriptor type associated with a buffer resource
           via a buffer view that formatted load, store, and atomic operations can be performed on. */

        template<uint32_t Size>
        class StorageTexelBufferArray : public TDescriptorArray<Size>
        {
        public:
            StorageTexelBufferArray(uint32_t binding) noexcept:
                TDescriptorArray<Size>(VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, binding) {}
            DescriptorArray::BufferDescriptor operator[](uint32_t index) noexcept;
        };

        /* A uniform buffer is a descriptor type associated with a buffer resource directly,
           described in a shader as a structure with various members that load operations can be performed on. */

        template<uint32_t Size>
        class UniformBufferArray : public TDescriptorArray<Size>
        {
        public:
            UniformBufferArray(uint32_t binding) noexcept:
                TDescriptorArray<Size>(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, binding) {}
            DescriptorArray::BufferDescriptor operator[](uint32_t index) noexcept;
        };

        /* A storage buffer is a descriptor type associated with a buffer resource directly,
           described in a shader as a structure with various members that load, store,
           and atomic operations can be performed on. */

        template<uint32_t Size>
        class StorageBufferArray : public TDescriptorArray<Size>
        {
        public:
            StorageBufferArray(uint32_t binding) noexcept:
                TDescriptorArray<Size>(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, binding) {}
            DescriptorArray::BufferDescriptor operator[](uint32_t index) noexcept;
        };

        /* A dynamic uniform buffer is almost identical to a uniform buffer,
           and differs only in how the offset into the buffer is specified.
           The base offset calculated by the VkDescriptorBufferInfo when
           initially updating the descriptor set is added to a dynamic offset
           when binding the descriptor set. */

        template<uint32_t Size>
        class DynamicUniformBufferArray : public TDescriptorArray<Size>
        {
        public:
            DynamicUniformBufferArray(uint32_t binding) noexcept:
                TDescriptorArray<Size>(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, binding) {}
            DescriptorArray::BufferDescriptor operator[](uint32_t index) noexcept;
        };

        /* A dynamic storage buffer is almost identical to a storage buffer,
           and differs only in how the offset into the buffer is specified.
           The base offset calculated by the VkDescriptorBufferInfo when
           initially updating the descriptor set is added to a dynamic offset
           when binding the descriptor set. */

        template<uint32_t Size>
        class DynamicStorageBufferArray : public TDescriptorArray<Size>
        {
        public:
            DynamicStorageBufferArray(uint32_t binding) noexcept:
                TDescriptorArray<Size>(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, binding) {}
            DescriptorArray::BufferDescriptor operator[](uint32_t index) noexcept;
        };
    } // namespace descriptor
} // namespace magma

#include "bufferDescriptorArray.inl"
