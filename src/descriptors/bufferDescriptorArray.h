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
#include "array/bufferDescriptor.h"

namespace magma
{
    namespace descriptor
    {
        /* Base class of buffer descriptor array. */

        template<uint32_t Size>
        class BufferDescriptorArray : public DescriptorArray<VkDescriptorBufferInfo, Size>
        {
        public:
            bool associatedWithResource() const noexcept override;

        protected:
            typedef DescriptorArray<VkDescriptorBufferInfo, Size> Super;
            BufferDescriptorArray(VkDescriptorType descriptorType, uint32_t binding) noexcept;
            void write(VkDescriptorSet dstSet,
                VkWriteDescriptorSet& writeDescriptorSet) const noexcept override;
            array::BufferDescriptor getArrayElement(uint32_t index,
                VkBufferUsageFlags requiredUsage) noexcept;
        };

        /* Base class of texel buffer descriptor array. */

        template<uint32_t Size>
        class TexelBufferDescriptorArray : public DescriptorArray<VkBufferView, Size>
        {
        public:
            bool associatedWithResource() const noexcept override;

        protected:
            typedef DescriptorArray<VkBufferView, Size> Super;
            TexelBufferDescriptorArray(VkDescriptorType descriptorType, uint32_t binding) noexcept;
            void write(VkDescriptorSet dstSet,
                VkWriteDescriptorSet& writeDescriptorSet) const noexcept override;
            array::TexelBufferDescriptor getArrayElement(uint32_t index,
                VkBufferUsageFlags requiredUsage) noexcept;
        };

        /* A uniform texel buffer is a descriptor type associated with
           a buffer resource via a buffer view that formatted load
           operations can be performed on. */

        template<uint32_t Size>
        class UniformTexelBufferArray : public TexelBufferDescriptorArray<Size>
        {
        public:
            UniformTexelBufferArray(uint32_t binding) noexcept:
                TexelBufferDescriptorArray<Size>(VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, binding) {}
            array::TexelBufferDescriptor operator[](uint32_t index) noexcept;
        };

        /* A storage texel buffer is a descriptor type associated with
           a buffer resource via a buffer view that formatted load, store,
           and atomic operations can be performed on. */

        template<uint32_t Size>
        class StorageTexelBufferArray : public TexelBufferDescriptorArray<Size>
        {
        public:
            StorageTexelBufferArray(uint32_t binding) noexcept:
                TexelBufferDescriptorArray<Size>(VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, binding) {}
            array::TexelBufferDescriptor operator[](uint32_t index) noexcept;
        };

        /* A uniform buffer is a descriptor type associated with a buffer
           resource directly, described in a shader as a structure with
           various members that load operations can be performed on. */

        template<uint32_t Size>
        class UniformBufferArray : public BufferDescriptorArray<Size>
        {
        public:
            UniformBufferArray(uint32_t binding) noexcept:
                BufferDescriptorArray<Size>(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, binding) {}
            array::BufferDescriptor operator[](uint32_t index) noexcept;
        };

        /* A storage buffer is a descriptor type associated with a buffer
           resource directly, described in a shader as a structure with
           various members that load, store, and atomic operations can be
           performed on. */

        template<uint32_t Size>
        class StorageBufferArray : public BufferDescriptorArray<Size>
        {
        public:
            StorageBufferArray(uint32_t binding) noexcept:
                BufferDescriptorArray<Size>(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, binding) {}
            array::BufferDescriptor operator[](uint32_t index) noexcept;
        };

        /* A dynamic uniform buffer is almost identical to a uniform buffer,
           and differs only in how the offset into the buffer is specified.
           The base offset calculated by the VkDescriptorBufferInfo when
           initially updating the descriptor set is added to a dynamic offset
           when binding the descriptor set. */

        template<uint32_t Size>
        class DynamicUniformBufferArray : public BufferDescriptorArray<Size>
        {
        public:
            DynamicUniformBufferArray(uint32_t binding) noexcept:
                BufferDescriptorArray<Size>(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, binding) {}
            array::BufferDescriptor operator[](uint32_t index) noexcept;
        };

        /* A dynamic storage buffer is almost identical to a storage buffer,
           and differs only in how the offset into the buffer is specified.
           The base offset calculated by the VkDescriptorBufferInfo when
           initially updating the descriptor set is added to a dynamic offset
           when binding the descriptor set. */

        template<uint32_t Size>
        class DynamicStorageBufferArray : public BufferDescriptorArray<Size>
        {
        public:
            DynamicStorageBufferArray(uint32_t binding) noexcept:
                BufferDescriptorArray<Size>(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, binding) {}
            array::BufferDescriptor operator[](uint32_t index) noexcept;
        };
    } // namespace descriptor
} // namespace magma

#include "bufferDescriptorArray.inl"
