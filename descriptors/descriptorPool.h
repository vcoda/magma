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

namespace magma
{
    /* Pool descriptor contains a descriptor type and number of descriptors
       of that type to be allocated in the pool. */

    namespace descriptor
    {
        struct DescriptorPool : VkDescriptorPoolSize
        {
            constexpr DescriptorPool(const VkDescriptorType type, const uint32_t descriptorCount) noexcept:
                VkDescriptorPoolSize{type, descriptorCount} {}
        };

        struct SamplerPool : DescriptorPool
        {
            constexpr SamplerPool(const uint32_t count) noexcept:
                DescriptorPool(VK_DESCRIPTOR_TYPE_SAMPLER, count) {}
        };

        struct CombinedImageSamplerPool : DescriptorPool
        {
            constexpr CombinedImageSamplerPool(const uint32_t count) noexcept:
                DescriptorPool(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, count) {}
        };

        struct SampledImagePool : DescriptorPool
        {
            constexpr SampledImagePool(const uint32_t count) noexcept:
                DescriptorPool(VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, count) {}
        };

        struct StorageImagePool : DescriptorPool
        {
            constexpr StorageImagePool(const uint32_t count) noexcept:
                DescriptorPool(VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, count) {}
        };

        struct UniformTexelBufferPool : DescriptorPool
        {
            constexpr UniformTexelBufferPool(const uint32_t count) noexcept:
                DescriptorPool(VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, count) {}
        };

        struct StorageTexelBufferPool : DescriptorPool
        {
            constexpr StorageTexelBufferPool(const uint32_t count) noexcept:
                DescriptorPool(VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, count) {}
        };

        struct UniformBufferPool : DescriptorPool
        {
            constexpr UniformBufferPool(const uint32_t count) noexcept:
                DescriptorPool(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, count) {}
        };

        struct StorageBufferPool : DescriptorPool
        {
            constexpr StorageBufferPool(const uint32_t count) noexcept:
                DescriptorPool(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, count) {}
        };

        struct DynamicUniformBufferPool : DescriptorPool
        {
            constexpr DynamicUniformBufferPool(const uint32_t count) noexcept:
                DescriptorPool(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, count) {}
        };

        struct DynamicStorageBufferPool : DescriptorPool
        {
            constexpr DynamicStorageBufferPool(const uint32_t count) noexcept:
                DescriptorPool(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, count) {}
        };

        struct InputAttachmentPool : DescriptorPool
        {
            constexpr InputAttachmentPool(const uint32_t count) noexcept:
                DescriptorPool(VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, count) {}
        };

    #ifdef VK_EXT_inline_uniform_block
        template<typename UniformBlockType>
        struct InlineUniformBlockPool : DescriptorPool
        {
            constexpr InlineUniformBlockPool() noexcept:
                DescriptorPool(VK_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK_EXT, sizeof(UniformBlockType)) {}
        };
    #endif // VK_EXT_inline_uniform_block

    #ifdef VK_NV_ray_tracing
        struct AccelerationStructurePool : DescriptorPool
        {
            constexpr AccelerationStructurePool(const uint32_t count) noexcept:
                DescriptorPool(VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_NV, count) {}
        };
    #endif // VK_NV_ray_tracing

    #ifdef VK_VALVE_mutable_descriptor_type
        struct MutableDescriptorPool : DescriptorPool
        {
            constexpr MutableDescriptorPool(const uint32_t count) noexcept:
                DescriptorPool(VK_DESCRIPTOR_TYPE_MUTABLE_VALVE, count) {}
        };
    #endif // VK_VALVE_mutable_descriptor_type

    #ifdef VK_QCOM_image_processing
        struct SampleWeightImagePool : DescriptorPool
        {
            constexpr SampleWeightImagePool(const uint32_t count) noexcept:
                DescriptorPool(VK_DESCRIPTOR_TYPE_SAMPLE_WEIGHT_IMAGE_QCOM, count) {}
        };

        struct BlockMatchImagePool : DescriptorPool
        {
            constexpr BlockMatchImagePool(const uint32_t count) noexcept:
                DescriptorPool(VK_DESCRIPTOR_TYPE_BLOCK_MATCH_IMAGE_QCOM, count) {}
        };
    #endif // VK_QCOM_image_processing
    } // namespace descriptor
} // namespace magma
