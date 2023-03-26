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
#include "../../objects/buffer.h"
#include "../../objects/bufferView.h"

namespace magma
{
    namespace descriptor
    {
        namespace array
        {
            /* Descriptor element of buffer array. */

            class BufferDescriptor : Descriptor<VkDescriptorBufferInfo>
            {
            public:
                explicit BufferDescriptor(VkDescriptorBufferInfo& descriptor,
                    VkBufferUsageFlags requiredUsage, bool& updated) noexcept;
                void operator=(std::shared_ptr<const Buffer> buffer) noexcept;
            };

            /* Descriptor element of texel buffer array. */

            class TexelBufferDescriptor : Descriptor<VkBufferView>
            {
            public:
                explicit TexelBufferDescriptor(VkBufferView& descriptor,
                    VkBufferUsageFlags requiredUsage, bool& updated) noexcept;
                void operator=(std::shared_ptr<const BufferView> bufferView) noexcept;
            };
        } // namespace array
    } // namespace descriptor
} // namespace magma

#include "bufferDescriptor.inl"
