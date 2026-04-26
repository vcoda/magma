/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2026 Victor Coda.

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

namespace magma::descriptor
{
    /* Pool descriptor contains a descriptor type and number of
       descriptors of that type to be allocated in the pool. */

#define MAGMA_DEFINE_DESCRIPTOR_POOL_SIZE(Descriptor, descriptorType)\
    struct Descriptor##PoolSize : VkDescriptorPoolSize\
    {\
        constexpr Descriptor##PoolSize(uint32_t descriptorCount) noexcept:\
            VkDescriptorPoolSize{descriptorType, descriptorCount} {}\
    };

    MAGMA_DEFINE_DESCRIPTOR_POOL_SIZE(Sampler, VK_DESCRIPTOR_TYPE_SAMPLER)
    MAGMA_DEFINE_DESCRIPTOR_POOL_SIZE(CombinedImageSampler, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
    MAGMA_DEFINE_DESCRIPTOR_POOL_SIZE(SampledImage, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE)
    MAGMA_DEFINE_DESCRIPTOR_POOL_SIZE(StorageImage, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE)
    MAGMA_DEFINE_DESCRIPTOR_POOL_SIZE(UniformTexelBuffer, VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER)
    MAGMA_DEFINE_DESCRIPTOR_POOL_SIZE(StorageTexelBuffer, VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER)
    MAGMA_DEFINE_DESCRIPTOR_POOL_SIZE(UniformBuffer, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER)
    MAGMA_DEFINE_DESCRIPTOR_POOL_SIZE(StorageBuffer, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER)
    MAGMA_DEFINE_DESCRIPTOR_POOL_SIZE(DynamicUniformBuffer, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC)
    MAGMA_DEFINE_DESCRIPTOR_POOL_SIZE(DynamicStorageBuffer, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC)
    MAGMA_DEFINE_DESCRIPTOR_POOL_SIZE(InputAttachment, VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT)
#ifdef VK_KHR_acceleration_structure
    MAGMA_DEFINE_DESCRIPTOR_POOL_SIZE(AccelerationStructure, VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR)
#endif
#ifdef VK_VALVE_mutable_descriptor_type
    MAGMA_DEFINE_DESCRIPTOR_POOL_SIZE(MutableDescriptor, VK_DESCRIPTOR_TYPE_MUTABLE_VALVE)
#endif
#ifdef VK_QCOM_image_processing
    MAGMA_DEFINE_DESCRIPTOR_POOL_SIZE(SampleWeightImage, VK_DESCRIPTOR_TYPE_SAMPLE_WEIGHT_IMAGE_QCOM)
    MAGMA_DEFINE_DESCRIPTOR_POOL_SIZE(BlockMatchImage, VK_DESCRIPTOR_TYPE_BLOCK_MATCH_IMAGE_QCOM)
#endif

#ifdef VK_EXT_inline_uniform_block
    template<class UniformBlockType>
    struct InlineUniformBlockPoolSize : VkDescriptorPoolSize
    {
        constexpr InlineUniformBlockPoolSize() noexcept:
            VkDescriptorPoolSize(VK_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK_EXT, sizeof(UniformBlockType)) {}
    };
#endif // VK_EXT_inline_uniform_block
} // namespace magma::descriptor
