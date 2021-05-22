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
#include "baseDescriptorSet.h"
#include "../descriptors/bindings.h"
#include "../core/linearAllocator.h"

#ifdef VK_EXT_inline_uniform_block
#define MAGMA_MAX_INLINE_UNIFORM_BLOCK_POOL_SIZE 4096
#endif

namespace magma
{
    class Device;
    class DescriptorPool;
    class DescriptorSetLayout;
    class Buffer;
    class BufferView;
    class Sampler;
    class ImageView;
#ifdef VK_NV_ray_tracing
    class AccelerationStructure;
#endif
    class IAllocator;

    /*  A descriptor set object is an opaque object that contains storage for a set of descriptors,
        where the types and number of descriptors is defined by a descriptor set layout.
        The layout object may be used to define the association of each descriptor binding
        with memory or other implementation resources. The layout is used both for determining
        the resources that need to be associated with the descriptor set, and determining
        the interface between shader stages and shader resources. */

    class DescriptorSet : public BaseDescriptorSet
    {
    public:
        explicit DescriptorSet(std::shared_ptr<DescriptorPool> descriptorPool,
            std::shared_ptr<DescriptorSetLayout> setLayout,
            uint32_t maxDescriptorWrites = 16);
        ~DescriptorSet();
        void writeDescriptor(uint32_t binding,
            std::shared_ptr<const Buffer> buffer);
        void writeDescriptor(uint32_t binding,
            std::shared_ptr<const ImageView> imageView);
        void writeDescriptor(uint32_t binding,
            std::shared_ptr<const Sampler> sampler);
        void writeDescriptor(uint32_t binding,
            std::shared_ptr<const ImageView> imageView,
            std::shared_ptr<const Sampler> sampler);
        void writeDescriptor(uint32_t binding,
            std::shared_ptr<const BufferView> bufferView);
#ifdef VK_EXT_inline_uniform_block
        template<typename UniformBlockType>
        void writeInlineUniformDescriptor(uint32_t binding,
            const UniformBlockType& inlineUniformBlock);
#endif
#ifdef VK_NV_ray_tracing
        void writeDescriptor(uint32_t binding,
            std::shared_ptr<const AccelerationStructure> accelerationStructure);
#endif
        void writeDescriptorArray(uint32_t binding,
            const std::vector<std::shared_ptr<const Buffer>>& bufferArray);
        void writeDescriptorArray(uint32_t binding,
            const std::vector<std::shared_ptr<const ImageView>>& imageViewArray);
        void writeDescriptorArray(uint32_t binding,
            const std::vector<std::shared_ptr<const ImageView>>& imageViewArray,
            const std::vector<std::shared_ptr<const Sampler>>& samplerArray);
        void writeDescriptorArray(uint32_t binding,
            const std::vector<std::shared_ptr<const BufferView>>& bufferViewArray);
        virtual bool dirty() const noexcept override;
        virtual void update() override;

    private:
        void release();

        std::vector<VkDescriptorBufferInfo> bufferDescriptors;
        std::vector<VkDescriptorImageInfo> imageDescriptors;
        std::vector<VkDescriptorImageInfo> samplerDescriptors;
        std::vector<VkBufferView> bufferViews;
#ifdef VK_EXT_inline_uniform_block
        std::vector<VkWriteDescriptorSetInlineUniformBlockEXT> inlineUniformBlockDescriptors;
        std::unique_ptr<core::LinearAllocator> inlineUniformBlockAllocator;
#endif
#ifdef VK_NV_ray_tracing
        std::vector<VkWriteDescriptorSetAccelerationStructureNV> accelerationDescriptors;
        std::vector<VkAccelerationStructureNV> accelerationStructures;
#endif
        std::vector<VkWriteDescriptorSet> descriptorWrites;
        friend class DescriptorPool;
    };
} // namespace magma

#include "descriptorSet.inl"
