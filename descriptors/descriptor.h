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
#if defined(VK_KHR_acceleration_structure) || defined(VK_NV_ray_tracing)
    class AccelerationStructure;
#endif

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
            bool dirty() const noexcept { return updated; }
            virtual void getWriteDescriptor(VkDescriptorSet dstSet,
                VkWriteDescriptorSet& writeDescriptorSet) const noexcept;

        protected:
            Descriptor(VkDescriptorType descriptorType,
                uint32_t descriptorCount, uint32_t binding) noexcept;
            void updateSampler(std::shared_ptr<const magma::Sampler> sampler) noexcept;
            void updateImageView(std::shared_ptr<const ImageView> imageView,
                VkImageUsageFlags requiredUsage) noexcept;
            void updateBufferView(std::shared_ptr<const BufferView> bufferView,
                VkBufferUsageFlags requiredUsage) noexcept;
            void updateBuffer(std::shared_ptr<const Buffer> buffer,
                VkBufferUsageFlags requiredUsage) noexcept;

            union
            {
                VkDescriptorImageInfo imageDescriptor = {};
                VkDescriptorBufferInfo bufferDescriptor;
                VkBufferView texelBufferView;
            };

            VkDescriptorSetLayoutBinding binding;
            mutable bool updated;
        };

        /* A sampler descriptor is a descriptor type associated with a sampler object,
           used to control the behavior of sampling operations performed on a sampled image. */

        class Sampler : public Descriptor
        {
        public:
            Sampler(uint32_t binding) noexcept:
                Descriptor(VK_DESCRIPTOR_TYPE_SAMPLER, 1, binding) {}
            Sampler& operator=(std::shared_ptr<const magma::Sampler>) noexcept;
        };

        /* Immutable samplers are permanently bound into the set layout;
           later binding a sampler into an immutable sampler slot
           in a descriptor set is not allowed. */

        class ImmutableSampler : public Descriptor
        {
        public:
            ImmutableSampler(uint32_t binding) noexcept:
                Descriptor(VK_DESCRIPTOR_TYPE_SAMPLER, 1, binding) {}
            ImmutableSampler& operator=(std::shared_ptr<const magma::Sampler>) noexcept;
        };

        /* A combined image sampler is a single descriptor type associated with both a sampler and an image resource,
           combining both a sampler and sampled image descriptor into a single descriptor. */

        class CombinedImageSampler : public Descriptor
        {
        public:
            CombinedImageSampler(uint32_t binding) noexcept:
                Descriptor(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, binding) {}
            CombinedImageSampler& operator=(const std::pair<std::shared_ptr<const ImageView>, std::shared_ptr<const magma::Sampler>>&) noexcept;
        };

        /* Updates to a VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER descriptor with immutable samplers
           does not modify the samplers (the image views are updated, but the sampler updates are ignored). */

        class CombinedImageImmutableSampler : public Descriptor
        {
        public:
            CombinedImageImmutableSampler(uint32_t binding) noexcept:
                Descriptor(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, binding) {}
            CombinedImageImmutableSampler& operator=(const std::pair<std::shared_ptr<const ImageView>, std::shared_ptr<const magma::Sampler>>&) noexcept;
            CombinedImageImmutableSampler& operator=(std::shared_ptr<const ImageView>) noexcept;
        };

        /* A sampled image is a descriptor type associated with an image resource
           via an image view that sampling operations can be performed on. */

        class SampledImage : public Descriptor
        {
        public:
            SampledImage(uint32_t binding) noexcept:
                Descriptor(VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1, binding) {}
            SampledImage& operator=(std::shared_ptr<const ImageView>) noexcept;
        };

        /* A storage image is a descriptor type associated with an image resource
           via an image view that load, store, and atomic operations can be performed on. */

        class StorageImage : public Descriptor
        {
        public:
            StorageImage(uint32_t binding) noexcept:
                Descriptor(VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1, binding) {}
            StorageImage& operator=(std::shared_ptr<const ImageView>) noexcept;
        };

        /* A uniform texel buffer is a descriptor type associated with a buffer resource
           via a buffer view that formatted load operations can be performed on. */

        class UniformTexelBuffer : public Descriptor
        {
        public:
            UniformTexelBuffer(uint32_t binding) noexcept:
                Descriptor(VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1, binding) {}
            UniformTexelBuffer& operator=(std::shared_ptr<const BufferView>) noexcept;
        };

        /* A storage texel buffer is a descriptor type associated with a buffer resource
           via a buffer view that formatted load, store, and atomic operations can be performed on. */

        class StorageTexelBuffer : public Descriptor
        {
        public:
            StorageTexelBuffer(uint32_t binding) noexcept:
                Descriptor(VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1, binding) {}
            StorageTexelBuffer& operator=(std::shared_ptr<const BufferView>) noexcept;
        };

        /* A uniform buffer is a descriptor type associated with a buffer resource directly,
           described in a shader as a structure with various members that load operations can be performed on. */

        class UniformBuffer : public Descriptor
        {
        public:
            UniformBuffer(uint32_t binding) noexcept:
                Descriptor(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, binding) {}
            UniformBuffer& operator=(std::shared_ptr<const Buffer>) noexcept;
        };

        /* A storage buffer is a descriptor type associated with a buffer resource directly,
           described in a shader as a structure with various members that load, store,
           and atomic operations can be performed on. */

        class StorageBuffer : public Descriptor
        {
        public:
            StorageBuffer(uint32_t binding) noexcept:
                Descriptor(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, binding) {}
            StorageBuffer& operator=(std::shared_ptr<const Buffer>) noexcept;
        };

        /* A dynamic uniform buffer is almost identical to a uniform buffer,
           and differs only in how the offset into the buffer is specified.
           The base offset calculated by the VkDescriptorBufferInfo when
           initially updating the descriptor set is added to a dynamic offset
           when binding the descriptor set. */

        class DynamicUniformBuffer : public Descriptor
        {
        public:
            DynamicUniformBuffer(uint32_t binding) noexcept:
                Descriptor(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1, binding) {}
            DynamicUniformBuffer& operator=(std::shared_ptr<const Buffer>) noexcept;
        };

        /* A dynamic storage buffer is almost identical to a storage buffer,
           and differs only in how the offset into the buffer is specified.
           The base offset calculated by the VkDescriptorBufferInfo when
           initially updating the descriptor set is added to a dynamic offset
           when binding the descriptor set. */

        class DynamicStorageBuffer : public Descriptor
        {
        public:
            DynamicStorageBuffer(uint32_t binding) noexcept:
                Descriptor(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1, binding) {}
            DynamicStorageBuffer& operator=(std::shared_ptr<const Buffer>) noexcept;
        };

        /* An input attachment is a descriptor type associated with an image resource
           via an image view that can be used for framebuffer local load operations in fragment shaders. */

        class InputAttachment : public Descriptor
        {
        public:
            InputAttachment(uint32_t binding) noexcept:
                Descriptor(VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1, binding) {}
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
        template<class UniformBlockType>
        class InlineUniformBlock : public Descriptor
        {
        public:
            InlineUniformBlock(uint32_t binding) noexcept;
            void getWriteDescriptor(VkDescriptorSet dstSet,
                VkWriteDescriptorSet& writeDescriptorSet) const noexcept override;
            InlineUniformBlock<UniformBlockType>& operator=(const UniformBlockType&) noexcept;

        private:
            VkWriteDescriptorSetInlineUniformBlockEXT writeDescriptorSetInlineUniformBlock;
        };
    #endif // VK_EXT_inline_uniform_block

        /* An acceleration structure is a descriptor type that is used to retrieve
           scene geometry from within shaders that are used for ray traversal.
           Shaders have read-only access to the memory. */

    #if defined(VK_KHR_acceleration_structure) || defined(VK_NV_ray_tracing)
        class AccelerationStructure : public Descriptor
        {
        public:
            AccelerationStructure(uint32_t binding) noexcept;
            void getWriteDescriptor(VkDescriptorSet dstSet,
                VkWriteDescriptorSet& writeDescriptorSet) const noexcept override;
            AccelerationStructure& operator=(std::shared_ptr<const magma::AccelerationStructure>) noexcept;

        private:
        #ifdef VK_KHR_acceleration_structure
            VkAccelerationStructureKHR handle;
            VkWriteDescriptorSetAccelerationStructureKHR writeDescriptorSetAccelerationStructure;
        #else
            VkAccelerationStructureNV handle;
            VkWriteDescriptorSetAccelerationStructureNV writeDescriptorSetAccelerationStructure;
        #endif
        };
    #endif // VK_KHR_acceleration_structure || VK_NV_ray_tracing
    } // namespace descriptor
} // namespace magma

#include "descriptor.inl"
#include "inlineUniformBlockDescriptor.inl"
