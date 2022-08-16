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
#include "pch.h"
#pragma hdrstop
#include "rasterizationState.h"

namespace magma
{
hash_t RasterizationState::chainedHash() const noexcept
{
    hash_t hash = RasterizationState::hash();
    const VkBaseInStructure *next = reinterpret_cast<const VkBaseInStructure *>(pNext);
    while (next)
    {   // Hash depends on chained states order
        switch (next->sType)
        {
    #ifdef VK_AMD_rasterization_order
        case VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_RASTERIZATION_ORDER_AMD:
            {
                auto order = reinterpret_cast<const VkPipelineRasterizationStateRasterizationOrderAMD *>(next);
                hash = core::hashCombine(hash, core::hashArgs(
                    order->sType,
                    order->rasterizationOrder));
            }
    #endif // VK_AMD_rasterization_order
    #ifdef VK_EXT_conservative_rasterization
        case VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_CONSERVATIVE_STATE_CREATE_INFO_EXT:
            {
                auto conservative = reinterpret_cast<const VkPipelineRasterizationConservativeStateCreateInfoEXT *>(next);
                hash = core::hashCombine(hash, core::hashArgs(
                    conservative->sType,
                    conservative->flags,
                    conservative->conservativeRasterizationMode,
                    conservative->extraPrimitiveOverestimationSize));
            }
            break;
    #endif // VK_EXT_conservative_rasterization
    #ifdef VK_EXT_provoking_vertex
        case VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_PROVOKING_VERTEX_STATE_CREATE_INFO_EXT:
            {
                auto provoking = reinterpret_cast<const VkPipelineRasterizationProvokingVertexStateCreateInfoEXT *>(next);
                hash = core::hashCombine(hash, core::hashArgs(
                    provoking->sType,
                    provoking->provokingVertexMode));
            }
            break;
    #endif // VK_EXT_provoking_vertex
    #ifdef VK_EXT_transform_feedback
        case VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_STREAM_CREATE_INFO_EXT:
            {
                auto stream = reinterpret_cast<const VkPipelineRasterizationStateStreamCreateInfoEXT *>(next);
                hash = core::hashCombine(hash, core::hashArgs(
                    stream->sType,
                    stream->flags,
                    stream->rasterizationStream));
            }
            break;
    #endif // VK_EXT_transform_feedback
        // Add any new rasterization state descriptors here
        default:
            MAGMA_ASSERT(false && "unknown chained structure");
        }
        next = reinterpret_cast<const VkBaseInStructure *>(next->pNext);
    }
    return hash;
}
} // namespace magma
