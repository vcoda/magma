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

namespace magma
{
    class Sampler;
    class ImageView;
    class Buffer;

    namespace reflection
    {
        /* Object specifying a descriptor set layout binding. */

        class DescriptorSetLayoutBinding : public VkDescriptorSetLayoutBinding
        {
        public:
            ~DescriptorSetLayoutBinding();
            bool dirty() const noexcept { return updated; }

        protected:
            DescriptorSetLayoutBinding(VkDescriptorType descriptorType,
                uint32_t descriptorCount,
                uint32_t binding);

        protected:
            VkWriteDescriptorSet descriptorWrite;
            bool updated;
            friend class DescriptorSet;
        };

        class Sampler : public DescriptorSetLayoutBinding
        {
        public:
            Sampler(uint32_t binding = 0) noexcept:
                DescriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_SAMPLER, 1, binding) {}
            Sampler& operator=(std::shared_ptr<const magma::Sampler> sampler);

        private:
            VkDescriptorImageInfo imageDescriptor;
        };
    
        class CombinedImageSampler : public DescriptorSetLayoutBinding
        {
        public:
            CombinedImageSampler(uint32_t binding = 0) noexcept:
                DescriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, binding) {}
            CombinedImageSampler& operator=(const std::pair<std::shared_ptr<const ImageView>, std::shared_ptr<const magma::Sampler>>& combinedImageSampler);

        private:
            VkDescriptorImageInfo imageDescriptor;
        };
        
        class SampledImage : public DescriptorSetLayoutBinding
        {
        public:
            SampledImage(uint32_t binding = 0) noexcept:
                DescriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1, binding) {}
            SampledImage& operator=(std::shared_ptr<const ImageView> imageView);

        private:
            VkDescriptorImageInfo imageDescriptor;
        };
        
        class UniformBuffer  : public DescriptorSetLayoutBinding
        {
        public:
            UniformBuffer(uint32_t binding = 0) noexcept:
                DescriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, binding) {}
            UniformBuffer& operator=(std::shared_ptr<const Buffer> buffer);

        private:
            VkDescriptorBufferInfo bufferDescriptor;
        };
    } // namespace reflection
} // namespace magma
