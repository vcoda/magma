/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2019 Victor Coda.

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
    /* Descriptor contains a descriptor type and number of descriptors
       of that type to be allocated in the pool. */

    struct Descriptor : public VkDescriptorPoolSize
    {
        constexpr Descriptor(VkDescriptorType type, uint32_t descriptorCount):
            VkDescriptorPoolSize{}
        {
            this->type = type;
            this->descriptorCount = descriptorCount;
        }
    };

    namespace descriptors
    {
        struct Sampler : Descriptor
        {
            constexpr Sampler(uint32_t count): Descriptor(VK_DESCRIPTOR_TYPE_SAMPLER, count) {}
        };

        struct CombinedImageSampler : Descriptor
        {
            constexpr CombinedImageSampler(uint32_t count): Descriptor(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, count) {}
        };

        struct SampledImage : Descriptor
        {
            constexpr SampledImage(uint32_t count): Descriptor(VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, count) {}
        };

        struct StorageImage : Descriptor
        {
            constexpr StorageImage(uint32_t count): Descriptor(VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, count) {}
        };

        struct UniformTexelBuffer : Descriptor
        {
            constexpr UniformTexelBuffer(uint32_t count): Descriptor(VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, count) {}
        };

        struct StorageTexelBuffer : Descriptor
        {
            constexpr StorageTexelBuffer(uint32_t count): Descriptor(VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, count) {}
        };

        struct UniformBuffer : Descriptor
        {
            constexpr UniformBuffer(uint32_t count): Descriptor(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, count) {}
        };

        struct StorageBuffer : Descriptor
        {
            constexpr StorageBuffer(uint32_t count): Descriptor(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, count) {}
        };

        struct DynamicUniformBuffer : Descriptor
        {
            constexpr DynamicUniformBuffer(uint32_t count): Descriptor(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, count) {}
        };

        struct DynamicStorageBuffer : Descriptor
        {
            constexpr DynamicStorageBuffer(uint32_t count): Descriptor(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, count) {}
        };

        struct InputAttachment : Descriptor
        {
            constexpr InputAttachment(uint32_t count): Descriptor(VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, count) {}
        };
    } // namespace descriptors
} // namespace magma
