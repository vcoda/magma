/*
Magma - C++1x interface over Khronos Vulkan API.
Copyright (C) 2018 Victor Coda.

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
    struct Descriptor : public VkDescriptorPoolSize
    {
        Descriptor(VkDescriptorType type, uint32_t descriptorCount)
        {
            VkDescriptorPoolSize::type = type;
            VkDescriptorPoolSize::descriptorCount = descriptorCount;
        }
    };

    namespace descriptors
    {
        struct Sampler : Descriptor
        {
            Sampler(uint32_t count): Descriptor(VK_DESCRIPTOR_TYPE_SAMPLER, count) {}
        };

        struct CombinedImageSampler : Descriptor
        {
            CombinedImageSampler(uint32_t count): Descriptor(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, count) {}
        };

        struct SampledImage : Descriptor
        {
            SampledImage(uint32_t count): Descriptor(VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, count) {}
        };

        struct StorageImage : Descriptor
        {
            StorageImage(uint32_t count): Descriptor(VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, count) {}
        };

        struct UniformTexelBuffer : Descriptor
        {
            UniformTexelBuffer(uint32_t count): Descriptor(VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, count) {}
        };

        struct StorageTexelBuffer : Descriptor
        {
            StorageTexelBuffer(uint32_t count): Descriptor(VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, count) {}
        };

        struct UniformBuffer : Descriptor
        {
            UniformBuffer(uint32_t count): Descriptor(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, count) {}
        };

        struct StorageBuffer : Descriptor
        {
            StorageBuffer(uint32_t count): Descriptor(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, count) {}
        };

        struct DynamicUniformBuffer : Descriptor
        {
            DynamicUniformBuffer(uint32_t count): Descriptor(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, count) {}
        };

        struct DynamicStorageBuffer : Descriptor
        {
            DynamicStorageBuffer(uint32_t count): Descriptor(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, count) {}
        };

        struct InputAttachment : Descriptor
        {
            InputAttachment(uint32_t count): Descriptor(VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, count) {}
        };
    } // namespace descriptors
} // namespace magma
