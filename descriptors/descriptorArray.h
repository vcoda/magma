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
        /* Base class of descriptor array.
           Provides access to buffer and image array elements. */

        class DescriptorArray : public Descriptor
        {
        public:
            class ImageElement
            {
                const VkImageUsageFlags usage;
                VkDescriptorImageInfo& imageDescriptor;
                bool& updated;

            public:
                explicit ImageElement(VkImageUsageFlags, VkDescriptorImageInfo&, bool&) noexcept;
                void operator=(std::shared_ptr<const magma::Sampler>) noexcept;
                void operator=(std::shared_ptr<const ImageView>) noexcept;
            };

            class BufferElement
            {
                const VkBufferUsageFlags usage;
                VkDescriptorBufferInfo& bufferDescriptor;
                bool& updated;

            public:
                explicit BufferElement(VkBufferUsageFlags, VkDescriptorBufferInfo&, bool&) noexcept;
                void operator=(std::shared_ptr<const Buffer>) noexcept;
            };

        protected:
            DescriptorArray(VkDescriptorType descriptorType,
                uint32_t descriptorCount,
                uint32_t binding) noexcept;
        };

        /* Descriptor array. Provided template parameter to define an array size in compile-time. */

        template<uint32_t Size>
        class TDescriptorArray : public DescriptorArray
        {
        public:
            void getWriteDescriptor(VkDescriptorSet dstSet,
                VkWriteDescriptorSet& writeDescriptorSet) const noexcept override;

        protected:
            TDescriptorArray(VkDescriptorType descriptorType, uint32_t binding) noexcept:
                DescriptorArray(descriptorType, Size, binding) {}

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
        class SamplerArray : public TDescriptorArray<Size>
        {
        public:
            SamplerArray(uint32_t binding) noexcept:
                TDescriptorArray<Size>(VK_DESCRIPTOR_TYPE_SAMPLER, binding) {}
            DescriptorArray::ImageElement operator[](uint32_t index) noexcept;
        };

        /* Immutable samplers are permanently bound into the set layout;
           later binding a sampler into an immutable sampler slot
           in a descriptor set is not allowed. */

        template<uint32_t Size>
        class ImmutableSamplerArray : public TDescriptorArray<Size>
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
                TDescriptorArray<Size>(VK_DESCRIPTOR_TYPE_SAMPLER, binding)
            {   // If pImmutableSamplers is not NULL, then it is a pointer
                // to an array of sampler handles that will be copied
                // into the set layout and used for the corresponding binding.
                this->binding.pImmutableSamplers = immutableSamplers;
            }
            Descriptor operator[](uint32_t index) noexcept;

        private:
            VkSampler immutableSamplers[Size] = {};
        };

        /* A combined image sampler is a single descriptor type associated with both a sampler and an image resource,
           combining both a sampler and sampled image descriptor into a single descriptor. */

        template<uint32_t Size>
        class CombinedImageSamplerArray : public TDescriptorArray<Size>
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
                TDescriptorArray<Size>(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, binding) {}
            Descriptor operator[](uint32_t index) noexcept;
        };

        /* Updates to a VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER descriptor with immutable samplers
           does not modify the samplers (the image views are updated, but the sampler updates are ignored). */

        template<uint32_t Size>
        class CombinedImageImmutableSamplerArray : public TDescriptorArray<Size>
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
                TDescriptorArray<Size>(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, binding)
            {   // If pImmutableSamplers is not NULL, then it is a pointer
                // to an array of sampler handles that will be copied
                // into the set layout and used for the corresponding binding.
                this->binding.pImmutableSamplers = immutableSamplers;
            }
            Descriptor operator[](uint32_t index) noexcept;

        private:
            VkSampler immutableSamplers[Size] = {};
        };

        /* A sampled image is a descriptor type associated with an image resource
           via an image view that sampling operations can be performed on. */

        template<uint32_t Size>
        class SampledImageArray : public TDescriptorArray<Size>
        {
        public:
            SampledImageArray(uint32_t binding) noexcept:
                TDescriptorArray<Size>(VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, binding) {}
            DescriptorArray::ImageElement operator[](uint32_t index) noexcept;
        };

        /* A storage image is a descriptor type associated with an image resource
           via an image view that load, store, and atomic operations can be performed on. */

        template<uint32_t Size>
        class StorageImageArray : public TDescriptorArray<Size>
        {
        public:
            StorageImageArray(uint32_t binding) noexcept:
                TDescriptorArray<Size>(VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, binding) {}
            DescriptorArray::ImageElement operator[](uint32_t index) noexcept;
        };

        /* A uniform texel buffer is a descriptor type associated with a buffer resource
           via a buffer view that formatted load operations can be performed on. */

        template<uint32_t Size>
        class UniformTexelBufferArray : public TDescriptorArray<Size>
        {
        public:
            UniformTexelBufferArray(uint32_t binding) noexcept:
                TDescriptorArray<Size>(VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, binding) {}
            DescriptorArray::BufferElement operator[](uint32_t index) noexcept;
        };

        /* A storage texel buffer is a descriptor type associated with a buffer resource
           via a buffer view that formatted load, store, and atomic operations can be performed on. */

        template<uint32_t Size>
        class StorageTexelBufferArray : public TDescriptorArray<Size>
        {
        public:
            StorageTexelBufferArray(uint32_t binding) noexcept:
                TDescriptorArray<Size>(VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, binding) {}
            DescriptorArray::BufferElement operator[](uint32_t index) noexcept;
        };

        /* A uniform buffer is a descriptor type associated with a buffer resource directly,
           described in a shader as a structure with various members that load operations can be performed on. */

        template<uint32_t Size>
        class UniformBufferArray : public TDescriptorArray<Size>
        {
        public:
            UniformBufferArray(uint32_t binding) noexcept:
                TDescriptorArray<Size>(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, binding) {}
            DescriptorArray::BufferElement operator[](uint32_t index) noexcept;
        };

        /* A storage buffer is a descriptor type associated with a buffer resource directly,
           described in a shader as a structure with various members that load, store,
           and atomic operations can be performed on. */

        template<uint32_t Size>
        class StorageBufferArray : public TDescriptorArray<Size>
        {
        public:
            StorageBufferArray(uint32_t binding) noexcept:
                TDescriptorArray<Size>(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, binding) {}
            DescriptorArray::BufferElement operator[](uint32_t index) noexcept;
        };

        /* A dynamic uniform buffer is almost identical to a uniform buffer,
           and differs only in how the offset into the buffer is specified.
           The base offset calculated by the VkDescriptorBufferInfo when
           initially updating the descriptor set is added to a dynamic offset
           when binding the descriptor set. */

        template<uint32_t Size>
        class DynamicUniformBufferArray : public TDescriptorArray<Size>
        {
        public:
            DynamicUniformBufferArray(uint32_t binding) noexcept:
                TDescriptorArray<Size>(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, binding) {}
            DescriptorArray::BufferElement operator[](uint32_t index) noexcept;
        };

        /* A dynamic storage buffer is almost identical to a storage buffer,
           and differs only in how the offset into the buffer is specified.
           The base offset calculated by the VkDescriptorBufferInfo when
           initially updating the descriptor set is added to a dynamic offset
           when binding the descriptor set. */

        template<uint32_t Size>
        class DynamicStorageBufferArray : public TDescriptorArray<Size>
        {
        public:
            DynamicStorageBufferArray(uint32_t binding) noexcept:
                TDescriptorArray<Size>(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, binding) {}
            DescriptorArray::BufferElement operator[](uint32_t index) noexcept;
        };

        /* An input attachment is a descriptor type associated with an image resource
           via an image view that can be used for framebuffer local load operations in fragment shaders. */

        template<uint32_t Size>
        class InputAttachmentArray : public TDescriptorArray<Size>
        {
        public:
            InputAttachmentArray(uint32_t binding) noexcept:
                TDescriptorArray<Size>(VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, binding) {}
            DescriptorArray::ImageElement operator[](uint32_t index) noexcept;
        };
    } // namespace descriptor
} // namespace magma

#include "descriptorArray.inl"
