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
    class BufferView;
    class UniformTexelBuffer;
    class StorageTexelBuffer;
    class StorageBuffer;
#ifdef VK_NV_ray_tracing
    class AccelerationStructure;
#endif

    namespace reflection
    {
        /* Object specifying a descriptor set layout binding. */

        class DescriptorSetLayoutBinding : public VkDescriptorSetLayoutBinding
        {
        public:
            virtual ~DescriptorSetLayoutBinding() = default;
            const VkWriteDescriptorSet& getDescriptorWrite() const noexcept { return descriptorWrite; }
            bool dirty() const noexcept { return _dirty; }

        protected:
            DescriptorSetLayoutBinding(VkDescriptorType descriptorType,
                uint32_t descriptorCount,
                uint32_t binding);

        protected:
            VkWriteDescriptorSet descriptorWrite;
            bool _dirty;
            friend class DescriptorSet;
        };

        class ImageDescriptorBinding : public DescriptorSetLayoutBinding
        {
        protected:
            ImageDescriptorBinding(VkDescriptorType descriptorType, uint32_t descriptorCount, uint32_t binding) noexcept:
                DescriptorSetLayoutBinding(descriptorType, descriptorCount, binding) {}
            void writeDescriptor(const VkDescriptorImageInfo& imageDescriptor) noexcept;

        private:
            VkDescriptorImageInfo imageDescriptor;
        };

        class BufferDescriptorBinding : public DescriptorSetLayoutBinding
        {
        protected:
            BufferDescriptorBinding(VkDescriptorType descriptorType, uint32_t descriptorCount, uint32_t binding) noexcept:
                DescriptorSetLayoutBinding(descriptorType, descriptorCount, binding) {}
            void writeDescriptor(std::shared_ptr<const Buffer> buffer) noexcept;

        private:
            VkDescriptorBufferInfo bufferDescriptor;
        };

        class TexelBufferDescriptorBinding : public DescriptorSetLayoutBinding
        {
        protected:
            TexelBufferDescriptorBinding(VkDescriptorType descriptorType, uint32_t descriptorCount, uint32_t binding) noexcept:
                DescriptorSetLayoutBinding(descriptorType, descriptorCount, binding) {}
            void writeDescriptor(std::shared_ptr<const BufferView> bufferView) noexcept;

        private:
            VkBufferView texelBufferView;
        };

        class Sampler : public ImageDescriptorBinding
        {
        public:
            Sampler(uint32_t binding = 0) noexcept:
                ImageDescriptorBinding(VK_DESCRIPTOR_TYPE_SAMPLER, 1, binding) {}
            Sampler& operator=(std::shared_ptr<const magma::Sampler> sampler) noexcept;
        };
    
        class CombinedImageSampler : public ImageDescriptorBinding
        {
        public:
            CombinedImageSampler(uint32_t binding = 0) noexcept:
                ImageDescriptorBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, binding) {}
            CombinedImageSampler& operator=(const std::pair<std::shared_ptr<const ImageView>, std::shared_ptr<const magma::Sampler>>& combinedImageSampler) noexcept;
        };
        
        class SampledImage : public ImageDescriptorBinding
        {
        public:
            SampledImage(uint32_t binding = 0) noexcept:
                ImageDescriptorBinding(VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1, binding) {}
            SampledImage& operator=(std::shared_ptr<const ImageView> imageView) noexcept;
        };

        class StorageImage : public ImageDescriptorBinding
        {
        public:
            StorageImage(uint32_t binding = 0) noexcept:
                ImageDescriptorBinding(VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1, binding) {}
            StorageImage& operator=(std::shared_ptr<const ImageView> imageView) noexcept;
        };
        
        class UniformTexelBuffer : public TexelBufferDescriptorBinding
        {
        public:
            UniformTexelBuffer(uint32_t binding = 0) noexcept:
                TexelBufferDescriptorBinding(VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1, binding) {}
            UniformTexelBuffer& operator=(std::shared_ptr<const BufferView> bufferView) noexcept;
        };

        class StorageTexelBuffer : public TexelBufferDescriptorBinding
        {
        public:
            StorageTexelBuffer(uint32_t binding = 0) noexcept:
                TexelBufferDescriptorBinding(VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1, binding) {}
            StorageTexelBuffer& operator=(std::shared_ptr<const BufferView> bufferView) noexcept;
        };

        class UniformBuffer : public BufferDescriptorBinding
        {
        public:
            UniformBuffer(uint32_t binding = 0) noexcept:
                BufferDescriptorBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, binding) {}
            UniformBuffer& operator=(std::shared_ptr<const Buffer> buffer) noexcept;
        };

        class StorageBuffer : public BufferDescriptorBinding
        {
        public:
            StorageBuffer(uint32_t binding = 0) noexcept:
                BufferDescriptorBinding(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, binding) {}
            StorageBuffer& operator=(std::shared_ptr<const magma::StorageBuffer> buffer) noexcept;
        };

        class DynamicUniformBuffer : public BufferDescriptorBinding
        {
        public:
            DynamicUniformBuffer(uint32_t binding = 0) noexcept:
                BufferDescriptorBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1, binding) {}
            DynamicUniformBuffer& operator=(std::shared_ptr<const Buffer> buffer) noexcept;
        };

        class DynamicStorageBuffer : public BufferDescriptorBinding
        {
        public:
            DynamicStorageBuffer(uint32_t binding = 0) noexcept:
                BufferDescriptorBinding(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1, binding) {}
            DynamicStorageBuffer& operator=(std::shared_ptr<const magma::StorageBuffer> buffer) noexcept;
        };

#ifdef VK_EXT_inline_uniform_block
        template<typename UniformBlockType>
        class InlineUniformBlock : public DescriptorSetLayoutBinding
        {
        public:
            InlineUniformBlock(uint32_t binding = 0) noexcept;
            ~InlineUniformBlock();
            InlineUniformBlock& operator=(const UniformBlockType& inlineUniformBlock) noexcept;

        private:
            VkWriteDescriptorSetInlineUniformBlockEXT inlineUniformBlockDescriptorWrite;
        };
#endif // VK_EXT_inline_uniform_block

#ifdef VK_NV_ray_tracing
        class AccelerationStructure : public DescriptorSetLayoutBinding
        {
        public:
            AccelerationStructure(uint32_t binding = 0) noexcept:
                DescriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_NV, 1, binding) {}
            AccelerationStructure& operator=(std::shared_ptr<const magma::AccelerationStructure> accelerationStructure) noexcept;

        private:
            VkAccelerationStructureNV accelerationStructure = {};
            VkWriteDescriptorSetAccelerationStructureNV accelerationStructureDescriptorWrite = {};
        };
#endif // VK_NV_ray_tracing
    } // namespace reflection
} // namespace magma

#include "descriptorSetBinding.inl"
