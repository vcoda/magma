/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2024 Victor Coda.

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
#include "../misc/compatibility.h"

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
                auto rasterizationOrderInfo = reinterpret_cast<const VkPipelineRasterizationStateRasterizationOrderAMD *>(next);
                hash = core::hashCombine(hash, core::hashArgs(
                    rasterizationOrderInfo->sType,
                    rasterizationOrderInfo->rasterizationOrder));
            }
            break;
    #endif // VK_AMD_rasterization_order
    #ifdef VK_EXT_conservative_rasterization
        case VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_CONSERVATIVE_STATE_CREATE_INFO_EXT:
            {
                auto conservativeRasterizationInfo = reinterpret_cast<const VkPipelineRasterizationConservativeStateCreateInfoEXT *>(next);
                hash = core::hashCombine(hash, core::hashArgs(
                    conservativeRasterizationInfo->sType,
                    conservativeRasterizationInfo->flags,
                    conservativeRasterizationInfo->conservativeRasterizationMode,
                    conservativeRasterizationInfo->extraPrimitiveOverestimationSize));
            }
            break;
    #endif // VK_EXT_conservative_rasterization
    #ifdef VK_EXT_line_rasterization
        case VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_LINE_STATE_CREATE_INFO_EXT:
            {
                auto lineRasterizationInfo = reinterpret_cast<const VkPipelineRasterizationLineStateCreateInfoEXT *>(next);
                hash = core::hashCombine(hash, core::hashArgs(
                    lineRasterizationInfo->sType,
                    lineRasterizationInfo->lineRasterizationMode,
                    lineRasterizationInfo->stippledLineEnable,
                    lineRasterizationInfo->lineStippleFactor,
                    lineRasterizationInfo->lineStipplePattern));
            }
            break;
    #endif // VK_EXT_line_rasterization
    #ifdef VK_EXT_provoking_vertex
        case VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_PROVOKING_VERTEX_STATE_CREATE_INFO_EXT:
            {
                auto provokingVertexInfo = reinterpret_cast<const VkPipelineRasterizationProvokingVertexStateCreateInfoEXT *>(next);
                hash = core::hashCombine(hash, core::hashArgs(
                    provokingVertexInfo->sType,
                    provokingVertexInfo->provokingVertexMode));
            }
            break;
    #endif // VK_EXT_provoking_vertex
    #ifdef VK_EXT_transform_feedback
        case VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_STREAM_CREATE_INFO_EXT:
            {
                auto streamRasterizationInfo = reinterpret_cast<const VkPipelineRasterizationStateStreamCreateInfoEXT *>(next);
                hash = core::hashCombine(hash, core::hashArgs(
                    streamRasterizationInfo->sType,
                    streamRasterizationInfo->flags,
                    streamRasterizationInfo->rasterizationStream));
            }
            break;
    #endif // VK_EXT_transform_feedback
        // Add any new rasterization state descriptors here
        default:
            MAGMA_FAILURE("unknown chained structure");
        }
        next = reinterpret_cast<const VkBaseInStructure *>(next->pNext);
    }
    return hash;
}
} // namespace magma
