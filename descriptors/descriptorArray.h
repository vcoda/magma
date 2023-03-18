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
        /* Each individual descriptor binding is specified by a descriptor type,
           a count (array size) of the number of descriptors in the binding,
           a set of shader stages that can access the binding, and (if using immutable samplers)
           an array of sampler descriptors. */

        class DescriptorArray : public Descriptor
        {
        public:
            class ImageDescriptor;
            class ImmutableSamplerDescriptor;
            class ImageImmutableSamplerDescriptor;
            class BufferDescriptor;

        protected:
            DescriptorArray(VkDescriptorType descriptorType,
                uint32_t descriptorCount,
                uint32_t binding) noexcept
            : Descriptor(descriptorType, descriptorCount, binding) {}
        };

        /* Provides access to buffer or image array elements.
           Template parameter defines compile-time array size. */

        template<uint32_t Size>
        class TDescriptorArray : public DescriptorArray
        {
        public:
            constexpr uint32_t getArraySize() const { return Size; }
            void getWriteDescriptor(VkDescriptorSet dstSet,
                VkWriteDescriptorSet& writeDescriptorSet) const noexcept override;

        protected:
            TDescriptorArray(VkDescriptorType descriptorType, uint32_t binding) noexcept:
                DescriptorArray(descriptorType, Size, binding) {}
            ImageDescriptor getImageArrayElement(uint32_t index,
                VkImageUsageFlags requiredUsage) noexcept;
            BufferDescriptor getBufferArrayElement(uint32_t index,
                VkBufferUsageFlags requiredUsage) noexcept;

            union
            {
                std::array<VkDescriptorImageInfo, Size> imageDescriptors = {};
                std::array<VkDescriptorBufferInfo, Size> bufferDescriptors;
                std::array<VkBufferView, Size> texelBufferViews;
            };
        };
    } // namespace descriptor
} // namespace magma

#include "descriptorArray.inl"
