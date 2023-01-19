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
#include "descriptor.h"

namespace magma
{
    namespace descriptor
    {
        /* Base class of array of sampler descriptors. */

        template<uint32_t Size>
        struct DescriptorArray : public Descriptor
        {
        protected:
            DescriptorArray(VkDescriptorType descriptorType, uint32_t binding) noexcept:
                Descriptor(descriptorType, Size, binding) {}
            VkWriteDescriptorSet getWriteDescriptorSet(VkDescriptorSet dstSet) const noexcept override;

            union
            {
                VkDescriptorImageInfo imageDescriptors[Size] = {};
                VkDescriptorBufferInfo bufferDescriptors[Size];
                VkBufferView texelBufferViews[Size];
            };
        };

        /* A sampler descriptor is a descriptor type associated with a sampler object,
           used to control the behavior of sampling operations performed on a sampled image. */

        template<uint32_t Size>
        class SamplerArray : public DescriptorArray<Size>
        {
        public:
            struct Descriptor
            {
                Descriptor(VkDescriptorImageInfo& imageDescriptor) noexcept:
                    imageDescriptor(imageDescriptor) {}
                void operator=(std::shared_ptr<const magma::Sampler>) noexcept;
                VkDescriptorImageInfo& imageDescriptor;
            };

            SamplerArray(uint32_t binding): noexcept:
                DescriptorArray<Size>(VK_DESCRIPTOR_TYPE_SAMPLER, binding) {}
            Descriptor operator[](uint32_t index) noexcept;
        };

        /* Immutable samplers are permanently bound into the set layout;
           later binding a sampler into an immutable sampler slot
           in a descriptor set is not allowed. */

        template<uint32_t Size>
        class ImmutableSamplerArray : public DescriptorArray<Size>
        {
        public:
            struct Descriptor
            {
                Descriptor(VkDescriptorImageInfo& imageDescriptor, VkSampler& immutableSampler) noexcept:
                    imageDescriptor(imageDescriptor), immutableSampler(immutableSampler) {}
                void operator=(std::shared_ptr<const magma::Sampler>) noexcept;
                VkDescriptorImageInfo& imageDescriptor;
                VkSampler& immutableSampler;
            };

            ImmutableSamplerArray(uint32_t binding) noexcept:
                DescriptorArray<Size>(VK_DESCRIPTOR_TYPE_SAMPLER, binding)
            {
                DescriptorArray<Size>::binding.pImmutableSamplers = immutableSamplers;
            }
            Descriptor operator[](uint32_t index) noexcept;

        private:
            VkSampler immutableSamplers[Size] = {};
        };

        /* A combined image sampler is a single descriptor type associated with both a sampler and an image resource,
           combining both a sampler and sampled image descriptor into a single descriptor. */

        template<uint32_t Size>
        class CombinedImageSamplerArray : public DescriptorArray<Size>
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
                DescriptorArray<Size>(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, binding) {}
            Descriptor operator[](uint32_t index) noexcept;
        };

        /* Updates to a VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER descriptor with immutable samplers
           does not modify the samplers (the image views are updated, but the sampler updates are ignored). */

        template<uint32_t Size>
        class CombinedImageImmutableSamplerArray : public DescriptorArray<Size>
        {
        public:
            struct Descriptor
            {
                Descriptor(VkDescriptorImageInfo& imageDescriptor, VkSampler& immutableSampler) noexcept:
                    imageDescriptor(imageDescriptor), immutableSampler(immutableSampler) {}
                void operator=(const std::pair<std::shared_ptr<const ImageView>, std::shared_ptr<const magma::Sampler>>&) noexcept;
                void operator=(std::shared_ptr<const ImageView>) noexcept;
                VkDescriptorImageInfo& imageDescriptor;
                VkSampler& immutableSampler;
            };

            CombinedImageImmutableSamplerArray(uint32_t binding) noexcept:
                DescriptorArray<Size>(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, binding)
            {
                DescriptorArray<Size>::binding.pImmutableSamplers = immutableSamplers;
            }
            Descriptor operator[](uint32_t index) noexcept;

        private:
            VkSampler immutableSamplers[Size] = {};
        };

        /* A storage buffer is a descriptor type associated with a buffer resource directly,
           described in a shader as a structure with various members that load, store,
           and atomic operations can be performed on. */

        template<uint32_t Size>
        class StorageBufferArray : public DescriptorArray<Size>
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
                DescriptorArray<Size>(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, binding) {}
            Descriptor operator[](uint32_t index) noexcept;
        };
    } // namespace descriptor
} // namespace magma

#include "descriptorArray.inl"
