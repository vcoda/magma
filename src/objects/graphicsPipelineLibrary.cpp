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
#include "graphicsPipelineLibrary.h"
#include "pipelineLayout.h"
#include "device.h"
#include "renderPass.h"
#include "../shaders/pipelineShaderStage.h"
#include "../states/vertexInputState.h"
#include "../states/inputAssemblyState.h"
#include "../states/tesselationState.h"
#include "../states/viewportState.h"
#include "../states/rasterizationState.h"
#include "../states/multisampleState.h"
#include "../states/depthStencilState.h"
#include "../states/colorBlendState.h"
#include "../allocator/allocator.h"
#include "../exceptions/errorResult.h"

namespace magma
{
#ifdef VK_EXT_graphics_pipeline_library
GraphicsPipelineLibrary::GraphicsPipelineLibrary(std::shared_ptr<Device> device,
    std::shared_ptr<IAllocator> allocator /* nullptr */) noexcept:
    PipelineLibrary(std::move(device), std::move(allocator))
{}

void GraphicsPipelineLibrary::compileVertexInputInterface(const VertexInputState& vertexInputState,
    const InputAssemblyState& inputAssemblyState, VkPipelineCreateFlags flags /* 0 */)
{   // https://registry.khronos.org/vulkan/specs/1.3-extensions/html/vkspec.html#pipelines-graphics-subsets-vertex-input
    VkGraphicsPipelineCreateInfo graphicsPipelineInfo = {};
    VkGraphicsPipelineLibraryCreateInfoEXT graphicsPipelineLibraryInfo;
    graphicsPipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    graphicsPipelineInfo.pNext = &graphicsPipelineLibraryInfo;
    graphicsPipelineInfo.flags = flags | VK_PIPELINE_CREATE_LIBRARY_BIT_KHR;
    graphicsPipelineInfo.pVertexInputState = &vertexInputState;
    graphicsPipelineInfo.pInputAssemblyState = &inputAssemblyState;
    graphicsPipelineInfo.basePipelineIndex = -1;
    graphicsPipelineLibraryInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_LIBRARY_CREATE_INFO_EXT;
    graphicsPipelineLibraryInfo.pNext = nullptr;
    graphicsPipelineLibraryInfo.flags = VK_GRAPHICS_PIPELINE_LIBRARY_VERTEX_INPUT_INTERFACE_BIT_EXT;
    VkPipeline handle = 0;
    const VkResult result = vkCreateGraphicsPipelines(getNativeDevice(), VK_NULL_HANDLE,
        1, &graphicsPipelineInfo, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to compile vertex input interface");
    libraries.push_back(handle);
}

void GraphicsPipelineLibrary::compilePreRasterizationShaders(const std::vector<PipelineShaderStage>& preRasterizationShaderStages,
    const TesselationState& tesselationState, const ViewportState& viewportState,
    const RasterizationState& rasterizationState, const std::vector<VkDynamicState>& dynamicStates,
    const std::unique_ptr<PipelineLayout>& layout, VkPipelineCreateFlags flags /* 0 */)
{   // https://registry.khronos.org/vulkan/specs/1.3-extensions/html/vkspec.html#pipelines-graphics-subsets-pre-rasterization
    MAGMA_STACK_ARRAY(VkPipelineShaderStageCreateInfo, dereferencedStages, preRasterizationShaderStages.size());
    for (auto const& stage: preRasterizationShaderStages)
    {
        switch (stage.stage)
        {
        case VK_SHADER_STAGE_VERTEX_BIT:
        case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:
        case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT:
        case VK_SHADER_STAGE_GEOMETRY_BIT:
    #ifdef VK_EXT_mesh_shader
        case VK_SHADER_STAGE_TASK_BIT_EXT:
        case VK_SHADER_STAGE_MESH_BIT_EXT:
    #endif // VK_EXT_mesh_shader
            dereferencedStages.put(stage);
            break;
        default:
            MAGMA_ERROR("invalid shader stage for pre-rasterization subset");
        }
    }
    VkGraphicsPipelineCreateInfo graphicsPipelineInfo = {};
    VkGraphicsPipelineLibraryCreateInfoEXT graphicsPipelineLibraryInfo;
    VkPipelineDynamicStateCreateInfo pipelineDynamicStateInfo;
    graphicsPipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    graphicsPipelineInfo.pNext = &graphicsPipelineLibraryInfo;
    graphicsPipelineInfo.flags = flags | VK_PIPELINE_CREATE_LIBRARY_BIT_KHR;
    graphicsPipelineInfo.stageCount = dereferencedStages.count();
    graphicsPipelineInfo.pStages = dereferencedStages;
    graphicsPipelineInfo.pTessellationState = &tesselationState;
    graphicsPipelineInfo.pViewportState = &viewportState;
    graphicsPipelineInfo.pRasterizationState = &rasterizationState;
    graphicsPipelineInfo.pDynamicState = dynamicStates.empty() ? nullptr : &pipelineDynamicStateInfo;
    graphicsPipelineInfo.layout = *layout;
    // TODO: Do we need the following parameters?
    // Documentation states that pre-rasterization includes render pass and subpass.
    //graphicsPipelineInfo.renderPass = *renderPass;
    //graphicsPipelineInfo.subpass = subpass;
    graphicsPipelineInfo.basePipelineIndex = -1;
    graphicsPipelineLibraryInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_LIBRARY_CREATE_INFO_EXT;
    graphicsPipelineLibraryInfo.pNext = nullptr;
    graphicsPipelineLibraryInfo.flags = VK_GRAPHICS_PIPELINE_LIBRARY_PRE_RASTERIZATION_SHADERS_BIT_EXT;
    pipelineDynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    pipelineDynamicStateInfo.pNext = nullptr;
    pipelineDynamicStateInfo.flags = 0;
    pipelineDynamicStateInfo.dynamicStateCount = core::countof(dynamicStates);
    pipelineDynamicStateInfo.pDynamicStates = dynamicStates.data();
    VkPipeline handle = 0;
    const VkResult result = vkCreateGraphicsPipelines(getNativeDevice(), VK_NULL_HANDLE,
        1, &graphicsPipelineInfo, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to compile pre-rasterization shaders");
    libraries.push_back(handle);
}

void GraphicsPipelineLibrary::compileFragmentShader(const PipelineShaderStage& shaderStage,
    const MultisampleState& multisampleState, const DepthStencilState& depthStencilState,
    const std::unique_ptr<PipelineLayout>& layout, std::shared_ptr<RenderPass> renderPass, uint32_t subpass,
    VkPipelineCreateFlags flags /* 0 */)
{   // https://registry.khronos.org/vulkan/specs/1.3-extensions/html/vkspec.html#pipelines-graphics-subsets-fragment-shader
    if (shaderStage.stage != VK_SHADER_STAGE_FRAGMENT_BIT)
        MAGMA_ERROR("invalid shader stage for fragment subset");
    VkGraphicsPipelineCreateInfo graphicsPipelineInfo = {};
    VkGraphicsPipelineLibraryCreateInfoEXT graphicsPipelineLibraryInfo;
    graphicsPipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    graphicsPipelineInfo.pNext = &graphicsPipelineLibraryInfo;
    graphicsPipelineInfo.flags = flags | VK_PIPELINE_CREATE_LIBRARY_BIT_KHR;
    graphicsPipelineInfo.stageCount = 1;
    graphicsPipelineInfo.pStages = &shaderStage;
    if (renderPass || multisampleState.sampleShadingEnable)
        graphicsPipelineInfo.pMultisampleState = &multisampleState;
    graphicsPipelineInfo.pDepthStencilState = &depthStencilState;
    graphicsPipelineInfo.layout = *layout;
    graphicsPipelineInfo.renderPass = MAGMA_OPTIONAL_HANDLE(renderPass);
    graphicsPipelineInfo.subpass = subpass;
    graphicsPipelineInfo.basePipelineIndex = -1;
    graphicsPipelineLibraryInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_LIBRARY_CREATE_INFO_EXT;
    graphicsPipelineLibraryInfo.pNext = nullptr;
    graphicsPipelineLibraryInfo.flags = VK_GRAPHICS_PIPELINE_LIBRARY_FRAGMENT_SHADER_BIT_EXT;
    VkPipeline handle = 0;
    const VkResult result = vkCreateGraphicsPipelines(getNativeDevice(), VK_NULL_HANDLE,
        1, &graphicsPipelineInfo, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to compile fragment shader");
    libraries.push_back(handle);
}

void GraphicsPipelineLibrary::compileFragmentOutputInterface(const MultisampleState& multisampleState,
    const ColorBlendState& colorBlendState, std::shared_ptr<RenderPass> renderPass, uint32_t subpass,
    VkPipelineCreateFlags flags /* 0 */)
{   // https://registry.khronos.org/vulkan/specs/1.3-extensions/html/vkspec.html#pipelines-graphics-subsets-fragment-output
    VkGraphicsPipelineCreateInfo graphicsPipelineInfo = {};
    VkGraphicsPipelineLibraryCreateInfoEXT graphicsPipelineLibraryInfo;
    graphicsPipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    graphicsPipelineInfo.pNext = &graphicsPipelineLibraryInfo;
    graphicsPipelineInfo.flags = flags | VK_PIPELINE_CREATE_LIBRARY_BIT_KHR;
    graphicsPipelineInfo.pMultisampleState = &multisampleState;
    graphicsPipelineInfo.pColorBlendState = &colorBlendState;
    graphicsPipelineInfo.renderPass = *renderPass;
    graphicsPipelineInfo.subpass = subpass;
    graphicsPipelineInfo.basePipelineIndex = -1;
    graphicsPipelineLibraryInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_LIBRARY_CREATE_INFO_EXT;
    graphicsPipelineLibraryInfo.pNext = nullptr;
    graphicsPipelineLibraryInfo.flags = VK_GRAPHICS_PIPELINE_LIBRARY_FRAGMENT_OUTPUT_INTERFACE_BIT_EXT;
    VkPipeline handle = 0;
    const VkResult result = vkCreateGraphicsPipelines(getNativeDevice(), VK_NULL_HANDLE,
        1, &graphicsPipelineInfo, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to compile fragment output interface");
    libraries.push_back(handle);
}
#endif // VK_EXT_graphics_pipeline_library
} // namespace magma
