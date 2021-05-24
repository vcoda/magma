/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2020 Victor Coda.

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
#include "binding.h"

namespace magma
{
    class StorageBuffer;

    namespace binding
    {
        /* Object specifying a descriptor set layout binding array. */

        class DescriptorSetLayoutBindingArray : public DescriptorSetLayoutBinding
        {
        public:
            ~DescriptorSetLayoutBindingArray();

        protected:
            explicit DescriptorSetLayoutBindingArray(VkDescriptorType descriptorType,
                uint32_t binding) noexcept;
        };

        /* A storage buffer is a descriptor type associated with a buffer resource directly, 
           described in a shader as a structure with various members that load, store, 
           and atomic operations can be performed on. */

        class StorageBufferArray : public DescriptorSetLayoutBindingArray
        {
        public:
            StorageBufferArray(uint32_t binding) noexcept:
                DescriptorSetLayoutBindingArray(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, binding) {}
            StorageBufferArray& operator=(std::initializer_list<std::shared_ptr<const magma::StorageBuffer>> bufferArray) noexcept;
        };
    } // namespace binding
} // namespace magma
