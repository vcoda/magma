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
#include "descriptorArray.h"

namespace magma
{
    namespace descriptor
    {
        /* A sampler descriptor is a descriptor type associated with a sampler object,
           used to control the behavior of sampling operations performed on a sampled image. */

        template<uint32_t Size>
        class SamplerArray : public TDescriptorArray<Size>
        {
        public:
            SamplerArray(uint32_t binding) noexcept:
                TDescriptorArray<Size>(VK_DESCRIPTOR_TYPE_SAMPLER, binding) {}
            DescriptorArray::ImageDescriptor operator[](uint32_t index) noexcept;
        };

        /* Immutable samplers are permanently bound into the set layout;
           later binding a sampler into an immutable sampler slot
           in a descriptor set is not allowed. */

        template<uint32_t Size>
        class ImmutableSamplerArray : public TDescriptorArray<Size>
        {
        public:
            ImmutableSamplerArray(uint32_t binding) noexcept:
                TDescriptorArray<Size>(VK_DESCRIPTOR_TYPE_SAMPLER, binding)
            {   // If pImmutableSamplers is not NULL, then it is a pointer
                // to an array of sampler handles that will be copied
                // into the set layout and used for the corresponding binding.
                this->binding.pImmutableSamplers = immutableSamplers;
            }
            DescriptorArray::ImmutableSamplerDescriptor operator[](uint32_t index) noexcept;

        private:
            VkSampler immutableSamplers[Size] = {};
        };

        /* A combined image sampler is a single descriptor type associated with both a sampler and an image resource,
           combining both a sampler and sampled image descriptor into a single descriptor. */

        template<uint32_t Size>
        class CombinedImageSamplerArray : public TDescriptorArray<Size>
        {
        public:
            CombinedImageSamplerArray(uint32_t binding) noexcept:
                TDescriptorArray<Size>(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, binding) {}
            DescriptorArray::ImageDescriptor operator[](uint32_t index) noexcept;
        };

        /* Updates to a VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER descriptor with immutable samplers
           does not modify the samplers (the image views are updated, but the sampler updates are ignored). */

        template<uint32_t Size>
        class CombinedImageImmutableSamplerArray : public TDescriptorArray<Size>
        {
        public:
            CombinedImageImmutableSamplerArray(uint32_t binding) noexcept:
                TDescriptorArray<Size>(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, binding)
            {   // If pImmutableSamplers is not NULL, then it is a pointer
                // to an array of sampler handles that will be copied
                // into the set layout and used for the corresponding binding.
                this->binding.pImmutableSamplers = immutableSamplers;
            }
            DescriptorArray::ImageImmutableSamplerDescriptor operator[](uint32_t index) noexcept;

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
            DescriptorArray::ImageDescriptor operator[](uint32_t index) noexcept;
        };

        /* A storage image is a descriptor type associated with an image resource
           via an image view that load, store, and atomic operations can be performed on. */

        template<uint32_t Size>
        class StorageImageArray : public TDescriptorArray<Size>
        {
        public:
            StorageImageArray(uint32_t binding) noexcept:
                TDescriptorArray<Size>(VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, binding) {}
            DescriptorArray::ImageDescriptor operator[](uint32_t index) noexcept;
        };

        /* An input attachment is a descriptor type associated with an image resource
           via an image view that can be used for framebuffer local load operations in fragment shaders. */

        template<uint32_t Size>
        class InputAttachmentArray : public TDescriptorArray<Size>
        {
        public:
            InputAttachmentArray(uint32_t binding) noexcept:
                TDescriptorArray<Size>(VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, binding) {}
            DescriptorArray::ImageDescriptor operator[](uint32_t index) noexcept;
        };
    } // namespace descriptor
} // namespace magma

#include "imageDescriptorArray.inl"
