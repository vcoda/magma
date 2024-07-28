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
#include "pch.h"
#pragma hdrstop
#include "structureChain.h"

namespace magma
{
StructureChain::StructureChain(const StructureChain& other) noexcept:
    head(nullptr)
{
    if (other.head)
    {
        head = copyNode(other.head);
        for (auto src = other.head->pNext, dst = head;
            src && dst;
            src = src->pNext, dst = dst->pNext)
        {
            dst->pNext = copyNode(src);
        }
    }
}

void StructureChain::clear() noexcept
{
    while (head)
    {
        auto node = head;
        head = node->pNext;
        free(node);
    }
    head = nullptr;
}

hash_t StructureChain::hash() const noexcept
{
    hash_t hash = 0ull;
    for (auto node = head; node; node = node->pNext)
    {
        VkBaseOutStructure *next = node->pNext;
        node->pNext = nullptr; // Should not affect hash
        hash = core::hashCombine(hash, core::hashArray(
            (uint8_t *)node, getNodeSize(node->sType)));
        node->pNext = next;
    }
    return hash;
}

VkBaseOutStructure *StructureChain::getNode(VkStructureType sType) noexcept
{
    for (auto node = head; node; node = node->pNext)
    {
        if (node->sType == sType)
            return node;
    }
    return nullptr;
}

const VkBaseInStructure *StructureChain::getNode(VkStructureType sType) const noexcept
{
    for (auto node = head; node; node = node->pNext)
    {
        if (node->sType == sType)
            return reinterpret_cast<const VkBaseInStructure *>(node);
    }
    return nullptr;
}

VkBaseOutStructure *StructureChain::copyNode(const VkBaseOutStructure *src) noexcept
{
    MAGMA_ASSERT(src);
    const size_t size = getNodeSize(src->sType);
    MAGMA_ASSERT(size >= sizeof(VkBaseOutStructure));
    if (size < sizeof(VkBaseOutStructure))
        return nullptr; // Unknown structure
    auto dst = reinterpret_cast<VkBaseOutStructure *>(malloc(size));
    MAGMA_ASSERT(dst);
    if (dst)
    {   // Copy body, clear link
        memcpy(dst, src, size);
        dst->pNext = nullptr;
    }
    return dst;
}

#if defined(_MSC_VER)
    #pragma warning(disable: 4063) // case is not a valid value for switch of enum 'VkStructureType'
#elif defined(__GNUC__)
    #pragma GCC diagnostic ignored "-Wswitch" // warning: case value not in enumerated type 'VkStructureType'
#endif

size_t StructureChain::getNodeSize(VkStructureType sType) noexcept
{
    switch (sType)
    {
    case VK_STRUCTURE_TYPE_APPLICATION_INFO: return sizeof(VkApplicationInfo);
    case VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO: return sizeof(VkInstanceCreateInfo);
    case VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO: return sizeof(VkDeviceQueueCreateInfo);
    case VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO: return sizeof(VkDeviceCreateInfo);
    case VK_STRUCTURE_TYPE_SUBMIT_INFO: return sizeof(VkSubmitInfo);
    case VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO: return sizeof(VkMemoryAllocateInfo);
    case VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE: return sizeof(VkMappedMemoryRange);
    case VK_STRUCTURE_TYPE_BIND_SPARSE_INFO: return sizeof(VkBindSparseInfo);
    case VK_STRUCTURE_TYPE_FENCE_CREATE_INFO: return sizeof(VkFenceCreateInfo);
    case VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO: return sizeof(VkSemaphoreCreateInfo);
    case VK_STRUCTURE_TYPE_EVENT_CREATE_INFO: return sizeof(VkEventCreateInfo);
    case VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO: return sizeof(VkQueryPoolCreateInfo);
    case VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO: return sizeof(VkBufferCreateInfo);
    case VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO: return sizeof(VkBufferViewCreateInfo);
    case VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO: return sizeof(VkImageCreateInfo);
    case VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO: return sizeof(VkImageViewCreateInfo);
    case VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO: return sizeof(VkShaderModuleCreateInfo);
    case VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO: return sizeof(VkPipelineCacheCreateInfo);
    case VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO: return sizeof(VkPipelineShaderStageCreateInfo);
    case VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO: return sizeof(VkPipelineVertexInputStateCreateInfo);
    case VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO: return sizeof(VkPipelineInputAssemblyStateCreateInfo);
    case VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO: return sizeof(VkPipelineTessellationStateCreateInfo);
    case VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO: return sizeof(VkPipelineViewportStateCreateInfo);
    case VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO: return sizeof(VkPipelineRasterizationStateCreateInfo);
    case VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO: return sizeof(VkPipelineMultisampleStateCreateInfo);
    case VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO: return sizeof(VkPipelineDepthStencilStateCreateInfo);
    case VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO: return sizeof(VkPipelineColorBlendStateCreateInfo);
    case VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO: return sizeof(VkPipelineDynamicStateCreateInfo);
    case VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO: return sizeof(VkGraphicsPipelineCreateInfo);
    case VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO: return sizeof(VkComputePipelineCreateInfo);
    case VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO: return sizeof(VkPipelineLayoutCreateInfo);
    case VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO: return sizeof(VkSamplerCreateInfo);
    case VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO: return sizeof(VkDescriptorSetLayoutCreateInfo);
    case VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO: return sizeof(VkDescriptorPoolCreateInfo);
    case VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO: return sizeof(VkDescriptorSetAllocateInfo);
    case VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET: return sizeof(VkWriteDescriptorSet);
    case VK_STRUCTURE_TYPE_COPY_DESCRIPTOR_SET: return sizeof(VkCopyDescriptorSet);
    case VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO: return sizeof(VkFramebufferCreateInfo);
    case VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO: return sizeof(VkRenderPassCreateInfo);
    case VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO: return sizeof(VkCommandPoolCreateInfo);
    case VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO: return sizeof(VkCommandBufferAllocateInfo);
    case VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO: return sizeof(VkCommandBufferInheritanceInfo);
    case VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO: return sizeof(VkCommandBufferBeginInfo);
    case VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO: return sizeof(VkRenderPassBeginInfo);
    case VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER: return sizeof(VkBufferMemoryBarrier);
    case VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER: return sizeof(VkImageMemoryBarrier);
    case VK_STRUCTURE_TYPE_MEMORY_BARRIER: return sizeof(VkMemoryBarrier);
    case VK_STRUCTURE_TYPE_LOADER_INSTANCE_CREATE_INFO: return 0; // reserved
    case VK_STRUCTURE_TYPE_LOADER_DEVICE_CREATE_INFO: return 0; // reserved
#if VK_USE_64_BIT_PTR_DEFINES
    #include "sizeof64.inl"
#else
    #include "sizeof86.inl"
#endif
    }
}
} // namespace magma
