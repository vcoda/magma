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

namespace magma
{
    namespace descriptor
    {
        /* Each individual descriptor binding is specified by a descriptor type,
           a count (array size) of the number of descriptors in the binding,
           a set of shader stages that can access the binding, and (if using immutable samplers)
           an array of sampler descriptors. */

        class Descriptor
        {
        public:
            virtual ~Descriptor() = default;
            void setStageFlags(VkShaderStageFlags stageFlags) noexcept { binding.stageFlags = stageFlags; }
            VkShaderStageFlags getStageFlags() const noexcept { return binding.stageFlags; }
            const VkDescriptorSetLayoutBinding& getLayoutBinding() const noexcept { return binding; }
            VkImageType getImageType() const noexcept { return imageType; }
            bool dirty() const noexcept { return updated; }
            virtual void write(VkDescriptorSet dstSet,
                VkWriteDescriptorSet& writeDescriptorSet) const noexcept = 0;

        protected:
            Descriptor(VkDescriptorType descriptorType,
                uint32_t descriptorCount, uint32_t binding) noexcept;

            VkDescriptorSetLayoutBinding binding;
            VkImageType imageType = VK_IMAGE_TYPE_MAX_ENUM;
            mutable bool updated = false;
        };

        /* Base class of descriptor array. */

        class DescriptorArray : public Descriptor
        {
        public:
            class ImageDescriptor;
            class ImmutableSamplerDescriptor;
            class ImageImmutableSamplerDescriptor;
            class BufferDescriptor;
            class TexelBufferDescriptor;
            uint32_t getArraySize() const { return binding.descriptorCount; }

        protected:
            DescriptorArray(VkDescriptorType descriptorType,
                uint32_t descriptorCount, uint32_t binding) noexcept;
        };
    } // namespace descriptor
} // namespace magma
