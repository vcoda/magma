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
#include "../objects/image.h"
#include "../objects/imageView.h"
#include "../objects/buffer.h"
#include "../objects/bufferView.h"
#include "../objects/sampler.h"

namespace magma
{
    class DescriptorSet;
#ifdef VK_NV_ray_tracing
    class AccelerationStructure;
#endif

    namespace binding
    {
        /* Each individual descriptor binding is specified by a descriptor type,
           a count (array size) of the number of descriptors in the binding,
           a set of shader stages that can access the binding, and (if using immutable samplers)
           an array of sampler descriptors. */

        class DescriptorSetLayoutBinding : public VkDescriptorSetLayoutBinding
        {
        public:
            virtual ~DescriptorSetLayoutBinding() = default;
            virtual VkWriteDescriptorSet getWriteDescriptorSet(VkDescriptorSet dstSet) const noexcept;
            bool dirty() const noexcept { return updated; }

        protected:
            explicit DescriptorSetLayoutBinding(VkDescriptorType descriptorType,
                uint32_t descriptorCount, uint32_t binding) noexcept;

            union
            {
                VkDescriptorImageInfo imageDescriptor;
                VkDescriptorBufferInfo bufferDescriptor;
                VkBufferView texelBufferView;
            };
            mutable bool updated;
        };

        /* A sampler descriptor is a descriptor type associated with a sampler object,
           used to control the behavior of sampling operations performed on a sampled image. */

        class Sampler : public DescriptorSetLayoutBinding
        {
        public:
            Sampler(uint32_t binding) noexcept:
                DescriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_SAMPLER, 1, binding) {}
            Sampler& operator=(std::shared_ptr<const magma::Sampler>) noexcept;
        };

        /* Immutable samplers are permanently bound into the set layout;
           later binding a sampler into an immutable sampler slot
           in a descriptor set is not allowed. */

        class ImmutableSampler : public DescriptorSetLayoutBinding
        {
        public:
            ImmutableSampler(uint32_t binding) noexcept:
                DescriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_SAMPLER, 1, binding) {}
            ImmutableSampler& operator=(std::shared_ptr<const magma::Sampler>) noexcept;
        };

        /* A combined image sampler is a single descriptor type associated with both a sampler and an image resource,
           combining both a sampler and sampled image descriptor into a single descriptor. */

        class CombinedImageSampler : public DescriptorSetLayoutBinding
        {
        public:
            CombinedImageSampler(uint32_t binding) noexcept:
                DescriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, binding) {}
            CombinedImageSampler& operator=(const std::pair<std::shared_ptr<const ImageView>, std::shared_ptr<const magma::Sampler>>&) noexcept;
        };

        /* Updates to a VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER descriptor with immutable samplers
           does not modify the samplers (the image views are updated, but the sampler updates are ignored). */

        class CombinedImageImmutableSampler : public DescriptorSetLayoutBinding
        {
        public:
            CombinedImageImmutableSampler(uint32_t binding) noexcept:
                DescriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, binding) {}
            CombinedImageImmutableSampler& operator=(const std::pair<std::shared_ptr<const ImageView>, std::shared_ptr<const magma::Sampler>>&) noexcept;
            CombinedImageImmutableSampler& operator=(std::shared_ptr<const ImageView>) noexcept;
        };

        /* A sampled image is a descriptor type associated with an image resource
           via an image view that sampling operations can be performed on. */

        class SampledImage : public DescriptorSetLayoutBinding
        {
        public:
            SampledImage(uint32_t binding) noexcept:
                DescriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1, binding) {}
            SampledImage& operator=(std::shared_ptr<const ImageView>) noexcept;
        };

        /* A storage image is a descriptor type associated with an image resource
           via an image view that load, store, and atomic operations can be performed on. */

        class StorageImage : public DescriptorSetLayoutBinding
        {
        public:
            StorageImage(uint32_t binding) noexcept:
                DescriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1, binding) {}
            StorageImage& operator=(std::shared_ptr<const ImageView>) noexcept;
        };

        /* A uniform texel buffer is a descriptor type associated with a buffer resource
           via a buffer view that formatted load operations can be performed on. */

        class UniformTexelBuffer : public DescriptorSetLayoutBinding
        {
        public:
            UniformTexelBuffer(uint32_t binding) noexcept:
                DescriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1, binding) {}
            UniformTexelBuffer& operator=(std::shared_ptr<const BufferView>) noexcept;
        };

        /* A storage texel buffer is a descriptor type associated with a buffer resource
           via a buffer view that formatted load, store, and atomic operations can be performed on. */

        class StorageTexelBuffer : public DescriptorSetLayoutBinding
        {
        public:
            StorageTexelBuffer(uint32_t binding) noexcept:
                DescriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1, binding) {}
            StorageTexelBuffer& operator=(std::shared_ptr<const BufferView>) noexcept;
        };

        /* A uniform buffer is a descriptor type associated with a buffer resource directly,
           described in a shader as a structure with various members that load operations can be performed on. */

        class UniformBuffer : public DescriptorSetLayoutBinding
        {
        public:
            UniformBuffer(uint32_t binding) noexcept:
                DescriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, binding) {}
            UniformBuffer& operator=(std::shared_ptr<const Buffer>) noexcept;
        };

        /* A storage buffer is a descriptor type associated with a buffer resource directly,
           described in a shader as a structure with various members that load, store,
           and atomic operations can be performed on. */

        class StorageBuffer : public DescriptorSetLayoutBinding
        {
        public:
            StorageBuffer(uint32_t binding) noexcept:
                DescriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, binding) {}
            StorageBuffer& operator=(std::shared_ptr<const Buffer>) noexcept;
        };

        /* A dynamic uniform buffer is almost identical to a uniform buffer,
           and differs only in how the offset into the buffer is specified.
           The base offset calculated by the VkDescriptorBufferInfo when
           initially updating the descriptor set is added to a dynamic offset
           when binding the descriptor set. */

        class DynamicUniformBuffer : public DescriptorSetLayoutBinding
        {
        public:
            DynamicUniformBuffer(uint32_t binding) noexcept:
                DescriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1, binding) {}
            DynamicUniformBuffer& operator=(std::shared_ptr<const Buffer>) noexcept;
        };

        /* A dynamic storage buffer is almost identical to a storage buffer,
           and differs only in how the offset into the buffer is specified.
           The base offset calculated by the VkDescriptorBufferInfo when
           initially updating the descriptor set is added to a dynamic offset
           when binding the descriptor set. */

        class DynamicStorageBuffer : public DescriptorSetLayoutBinding
        {
        public:
            DynamicStorageBuffer(uint32_t binding) noexcept:
                DescriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1, binding) {}
            DynamicStorageBuffer& operator=(std::shared_ptr<const Buffer>) noexcept;
        };

        /* An input attachment is a descriptor type associated with an image resource
           via an image view that can be used for framebuffer local load operations in fragment shaders. */

        class InputAttachment : public DescriptorSetLayoutBinding
        {
        public:
            InputAttachment(uint32_t binding) noexcept:
                DescriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1, binding) {}
            InputAttachment& operator=(std::shared_ptr<const ImageView>) noexcept;
        };

        /* An inline uniform block is almost identical to a uniform buffer,
           and differs only in taking its storage directly from the encompassing
           descriptor set instead of being backed by buffer memory. It is typically
           used to access a small set of constant data that does not require
           the additional flexibility provided by the indirection enabled when using
           a uniform buffer where the descriptor and the referenced buffer memory are decoupled.
           Compared to push constants, they allow reusing the same set of constant data
           across multiple disjoint sets of drawing and dispatching commands. */

    #ifdef VK_EXT_inline_uniform_block
        template<typename UniformBlockType>
        class InlineUniformBlock : public DescriptorSetLayoutBinding
        {
        public:
            InlineUniformBlock(uint32_t binding) noexcept;
            InlineUniformBlock(const InlineUniformBlock<UniformBlockType>&) noexcept;
            ~InlineUniformBlock();
            VkWriteDescriptorSet getWriteDescriptorSet(VkDescriptorSet dstSet) const noexcept override;
            InlineUniformBlock<UniformBlockType>& operator=(const InlineUniformBlock<UniformBlockType>&) noexcept;
            InlineUniformBlock<UniformBlockType>& operator=(const UniformBlockType&) noexcept;

        private:
            VkWriteDescriptorSetInlineUniformBlockEXT writeDescriptorSetInlineUniformBlock = {};
        };
    #endif // VK_EXT_inline_uniform_block

        /* An acceleration structure is a descriptor type that is used to retrieve
           scene geometry from within shaders that are used for ray traversal.
           Shaders have read-only access to the memory. */

#ifdef VK_NV_ray_tracing
        class AccelerationStructure : public DescriptorSetLayoutBinding
        {
        public:
            AccelerationStructure(uint32_t binding) noexcept:
                DescriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_NV, 1, binding) {}
            VkWriteDescriptorSet getWriteDescriptorSet(VkDescriptorSet dstSet) const noexcept override;
            AccelerationStructure& operator=(std::shared_ptr<const magma::AccelerationStructure>) noexcept;

        private:
            VkAccelerationStructureNV accelerationStructure = {};
            VkWriteDescriptorSetAccelerationStructureNV writeDescriptorSetAccelerationStructure = {};
        };
#endif // VK_NV_ray_tracing
    } // namespace binding
} // namespace magma

#include "binding.inl"
#include "inlineUniformBlockBinding.inl"
