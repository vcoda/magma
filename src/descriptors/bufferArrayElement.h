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
#include "arrayElement.h"

namespace magma
{
    namespace descriptor
    {
        /* Element of array of buffer descriptors. */

        class BufferArrayElement final : ArrayElement
        {
        public:
            explicit BufferArrayElement(DescriptorSetLayoutBinding *array,
                VkDescriptorBufferInfo& element,
                VkBufferUsageFlags usage) noexcept;
            void operator=(std::shared_ptr<const Buffer> buffer) noexcept;

        private:
            VkDescriptorBufferInfo& element;
            const VkBufferUsageFlags usage;
        };

        /* Element of array of texel buffer descriptors. */

        class TexelBufferArrayElement final : ArrayElement
        {
        public:
            explicit TexelBufferArrayElement(DescriptorSetLayoutBinding *array,
                VkBufferView& element,
                VkBufferUsageFlags usage) noexcept;
            void operator=(std::shared_ptr<const BufferView> bufferView) noexcept;

        private:
            VkBufferView& element;
            const VkBufferUsageFlags usage;
        };
    } // namespace descriptor
} // namespace magma

#include "bufferArrayElement.inl"
