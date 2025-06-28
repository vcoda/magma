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

namespace magma::descriptor
{
    /* Element of array of buffer descriptors. */

    class BufferArrayElement final : ArrayElement<VkDescriptorBufferInfo>
    {
    public:
        explicit BufferArrayElement(DescriptorSetLayoutBinding *array,
            VkDescriptorBufferInfo& element,
            VkBufferUsageFlags usage) noexcept;
        void operator=(lent_ptr<const Buffer>) noexcept;
    };

    /* Element of array of texel buffer descriptors. */

    class TexelBufferArrayElement final : ArrayElement<VkBufferView>
    {
    public:
        explicit TexelBufferArrayElement(DescriptorSetLayoutBinding *array,
            VkBufferView& element,
            VkBufferUsageFlags usage) noexcept;
        void operator=(lent_ptr<const BufferView>) noexcept;
    };
} // namespace magma::descriptor

#include "bufferArrayElement.inl"
