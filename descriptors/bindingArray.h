/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2022 Victor Coda.

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
    namespace binding
    {
        /* An array of descriptors in the binding. */
 
        class DescriptorArraySetLayoutBinding : public DescriptorSetLayoutBinding
        {
        public:
            VkWriteDescriptorSet getWriteDescriptorSet(VkDescriptorSet dstSet) const noexcept override;
            ~DescriptorArraySetLayoutBinding();

        protected:
            DescriptorArraySetLayoutBinding(VkDescriptorType descriptorType,
                uint32_t descriptorCount, uint32_t binding) noexcept;

            union
            {	// Non-POD types (like std::vector) cannot be placed in the union
                VkDescriptorImageInfo *imageDescriptors = nullptr;
                VkDescriptorBufferInfo *bufferDescriptors;
                VkBufferView *texelBufferViews;
            };
        };

        /* A sampler descriptor is a descriptor type associated with a sampler object,
           used to control the behavior of sampling operations performed on a sampled image. */

        template<uint32_t Size>
        class SamplerArray : public DescriptorArraySetLayoutBinding
        {
        public:
            struct Descriptor
            {
                Descriptor(VkDescriptorImageInfo& imageDescriptor) noexcept:
                    imageDescriptor(imageDescriptor) {}
                void operator=(std::shared_ptr<const magma::Sampler>) noexcept;
                VkDescriptorImageInfo& imageDescriptor;
            };

            SamplerArray(uint32_t binding) noexcept:
                DescriptorArraySetLayoutBinding(VK_DESCRIPTOR_TYPE_SAMPLER, Size, binding) {}
            Descriptor operator[](uint32_t index) noexcept;
        };

        /* A combined image sampler is a single descriptor type associated with both a sampler and an image resource,
           combining both a sampler and sampled image descriptor into a single descriptor. */

		template<uint32_t Size>
        class CombinedImageSamplerArray : public DescriptorArraySetLayoutBinding
        {
        public:
            struct Descriptor
            {
                Descriptor(VkDescriptorImageInfo& imageDescriptor) noexcept:
                    imageDescriptor(imageDescriptor) {}
                void operator=(const std::pair<std::shared_ptr<const ImageView>, std::shared_ptr<const magma::Sampler>>&) noexcept;
                VkDescriptorImageInfo& imageDescriptor;
            };

            CombinedImageSamplerArray(uint32_t binding) noexcept:
                DescriptorArraySetLayoutBinding(VK_DESCRIPTOR_TYPE_SAMPLER, Size, binding) {}
            Descriptor operator[](uint32_t index) noexcept;
        };

        /* A storage buffer is a descriptor type associated with a buffer resource directly,
           described in a shader as a structure with various members that load, store,
           and atomic operations can be performed on. */

		template<uint32_t Size>
        class StorageBufferArray : public DescriptorArraySetLayoutBinding
        {
        public:
            struct Descriptor
            {
                Descriptor(VkDescriptorBufferInfo& bufferDescriptor) noexcept:
                    bufferDescriptor(bufferDescriptor) {}
                void operator=(std::shared_ptr<const Buffer>) noexcept;
                VkDescriptorBufferInfo& bufferDescriptor;
            };

            StorageBufferArray(uint32_t binding) noexcept:
                DescriptorArraySetLayoutBinding(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, Size, binding) {}
            Descriptor operator[](uint32_t index) noexcept;
        };
    } // namespace binding
} // namespace magma

#include "bindingArray.inl"
