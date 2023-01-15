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
        /* Object specifying a descriptor set layout binding array. */

        class DescriptorSetLayoutBindingArray : public DescriptorSetLayoutBinding
        {
        protected:
            DescriptorSetLayoutBindingArray(VkDescriptorType descriptorType,
                uint32_t binding) noexcept;
        };

        /* A combined image sampler is a single descriptor type associated with both a sampler and an image resource,
           combining both a sampler and sampled image descriptor into a single descriptor. */

        class CombinedImageSamplerArray : public DescriptorSetLayoutBindingArray
        {
        public:
            struct DescriptorImageInfo : VkDescriptorImageInfo
            {
                void operator=(const std::pair<std::shared_ptr<const ImageView>, std::shared_ptr<const magma::Sampler>>& combinedImageSampler) noexcept
                {
                    const VkDescriptorImageInfo imageDescriptor = combinedImageSampler.first->getDescriptor(combinedImageSampler.second);
                    memcpy(this, &imageDescriptor, sizeof(VkDescriptorImageInfo));
                }
            };

        public:
            CombinedImageSamplerArray(uint32_t binding) noexcept:
                DescriptorSetLayoutBindingArray(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, binding) {}
            DescriptorImageInfo& operator[](uint32_t index);

        private:
            std::vector<DescriptorImageInfo> imageDescriptors;
        };

        /* A storage buffer is a descriptor type associated with a buffer resource directly,
           described in a shader as a structure with various members that load, store,
           and atomic operations can be performed on. */

        class StorageBufferArray : public DescriptorSetLayoutBindingArray
        {
        public:
            struct DescriptorBufferInfo : VkDescriptorBufferInfo
            {
                void operator=(std::shared_ptr<const magma::Buffer> buffer) noexcept
                {
                    MAGMA_ASSERT(buffer->getUsage() & VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
                    const VkDescriptorBufferInfo bufferDescriptor = buffer->getDescriptor();
                    memcpy(this, &bufferDescriptor, sizeof(VkDescriptorBufferInfo));
                }
            };

        public:
            StorageBufferArray(uint32_t binding) noexcept:
                DescriptorSetLayoutBindingArray(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, binding) {}
            DescriptorBufferInfo& operator[](uint32_t index);

        private:
            std::vector<DescriptorBufferInfo> bufferDescriptors;
        };
    } // namespace binding
} // namespace magma
