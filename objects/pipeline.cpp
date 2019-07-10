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
#include "pipeline.h"
#include "pipelineLayout.h"
#include "pipelineCache.h"
#include "device.h"
#include "renderPass.h"
#include "../states/vertexInputState.h"
#include "../states/inputAssemblyState.h"
#include "../states/tesselationState.h"
#include "../states/viewportState.h"
#include "../states/rasterizationState.h"
#include "../states/multisampleState.h"
#include "../states/depthStencilState.h"
#include "../states/colorBlendState.h"
#include "../allocator/allocator.h"
#include "../misc/deviceExtension.h"
#include "../helpers/stackArray.h"

namespace magma
{
Pipeline::Pipeline(std::shared_ptr<Device> device, std::shared_ptr<IAllocator> allocator):
    NonDispatchable<VkPipeline>(VK_OBJECT_TYPE_PIPELINE, std::move(device), std::move(allocator))
{}

Pipeline::~Pipeline()
{
    vkDestroyPipeline(MAGMA_HANDLE(device), handle, MAGMA_OPTIONAL_INSTANCE(allocator));
}

VkShaderStatisticsInfoAMD Pipeline::getShaderStatistics(VkShaderStageFlagBits stage) const
{
    MAGMA_DEVICE_EXTENSION(vkGetShaderInfoAMD, VK_AMD_SHADER_INFO_EXTENSION_NAME);
    size_t infoSize = sizeof(VkShaderStatisticsInfoAMD);
    VkShaderStatisticsInfoAMD info;
    const VkResult getStatistics = vkGetShaderInfoAMD(MAGMA_HANDLE(device), handle, stage, VK_SHADER_INFO_TYPE_STATISTICS_AMD, &infoSize, &info);
    MAGMA_THROW_FAILURE(getStatistics, "failed to get shader statistics");
    return info;
}

std::vector<uint8_t> Pipeline::getShaderBinary(VkShaderStageFlagBits stage) const
{
    MAGMA_DEVICE_EXTENSION(vkGetShaderInfoAMD, VK_AMD_SHADER_INFO_EXTENSION_NAME);
    size_t binarySize;
    const VkResult getSize = vkGetShaderInfoAMD(MAGMA_HANDLE(device), handle, stage, VK_SHADER_INFO_TYPE_BINARY_AMD, &binarySize, nullptr);
    if (VK_SUCCESS == getSize)
    {
        std::vector<uint8_t> binary(binarySize);
        const VkResult getBinary = vkGetShaderInfoAMD(MAGMA_HANDLE(device), handle, stage, VK_SHADER_INFO_TYPE_BINARY_AMD, &binarySize, binary.data());
        if (VK_SUCCESS == getBinary)
            return binary;
    }
    return {};
}

std::string Pipeline::getShaderDisassembly(VkShaderStageFlagBits stage) const
{
    MAGMA_DEVICE_EXTENSION(vkGetShaderInfoAMD, VK_AMD_SHADER_INFO_EXTENSION_NAME);
    size_t disassemblySize;
    const VkResult getSize = vkGetShaderInfoAMD(MAGMA_HANDLE(device), handle, stage, VK_SHADER_INFO_TYPE_DISASSEMBLY_AMD, &disassemblySize, nullptr);
    if (VK_SUCCESS == getSize)
    {
        std::vector<char> disassembly(disassemblySize, '\0'); // May be large enough, so avoid stack allocation
        const VkResult getDisassembly = vkGetShaderInfoAMD(MAGMA_HANDLE(device), handle, stage, VK_SHADER_INFO_TYPE_DISASSEMBLY_AMD, &disassemblySize, disassembly.data());
        if (VK_SUCCESS == getDisassembly)
            return std::string(&disassembly[0]);
    }
    return std::string();
}

GraphicsPipeline::GraphicsPipeline(std::shared_ptr<Device> device, std::shared_ptr<const PipelineCache> pipelineCache,
    const std::vector<PipelineShaderStage>& stages,
    const VertexInputState& vertexInputState,
    const InputAssemblyState& inputAssemblyState,
    const RasterizationState& rasterizationState,
    const MultisampleState& multisampleState,
    const DepthStencilState& depthStencilState,
    const ColorBlendState& colorBlendState,
    const std::initializer_list<VkDynamicState>& dynamicStates,
    std::shared_ptr<const PipelineLayout> layout,
    std::shared_ptr<const RenderPass> renderPass,
    uint32_t subpass /* 0 */,
    std::shared_ptr<const GraphicsPipeline> basePipeline /* nullptr */,
    VkPipelineCreateFlags flags /* 0 */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    GraphicsPipeline(std::move(device), std::move(pipelineCache), stages,
        vertexInputState, inputAssemblyState,
        TesselationState(), // No tesselation state
        ViewportState(), // No viewport state (supposed to be dynamic)
        rasterizationState, multisampleState, depthStencilState, colorBlendState, dynamicStates,
        std::move(layout), std::move(renderPass), subpass, std::move(basePipeline), flags,
        std::move(allocator))
{}

GraphicsPipeline::GraphicsPipeline(std::shared_ptr<Device> device, std::shared_ptr<const PipelineCache> pipelineCache,
    const std::vector<PipelineShaderStage>& stages,
    const VertexInputState& vertexInputState,
    const InputAssemblyState& inputAssemblyState,
    const TesselationState& tesselationState,
    const ViewportState& viewportState,
    const RasterizationState& rasterizationState,
    const MultisampleState& multisampleState,
    const DepthStencilState& depthStencilState,
    const ColorBlendState& colorBlendState,
    const std::initializer_list<VkDynamicState>& dynamicStates,
    std::shared_ptr<const PipelineLayout> layout,
    std::shared_ptr<const RenderPass> renderPass,
    uint32_t subpass /* 0 */,
    std::shared_ptr<const GraphicsPipeline> basePipeline /* nullptr */,
    VkPipelineCreateFlags flags /* 0 */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    Pipeline(std::move(device), std::move(allocator))
{
    if (stages.empty())
        MAGMA_THROW("shader stages are empty");
    VkPipelineVertexInputStateCreateInfo pipelineVertexInput = {0};
    VkVertexInputBindingDescription vertexBindingDesc = {0};
    VkGraphicsPipelineCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = flags;
    if (basePipeline)
        info.flags |= VK_PIPELINE_CREATE_DERIVATIVE_BIT;
    MAGMA_STACK_ARRAY(VkPipelineShaderStageCreateInfo, dereferencedStages, stages.size());
    for (auto& stage : stages)
        dereferencedStages.put(stage);
    info.stageCount = MAGMA_COUNT(dereferencedStages);
    info.pStages = dereferencedStages;
    if (vertexInputState.vertexBindingDescriptionCount > 0)
        info.pVertexInputState = &vertexInputState;
    else
    {
        vertexBindingDesc = VertexInputBinding(0, vertexInputState.stride(0));
        pipelineVertexInput = vertexInputState;
        pipelineVertexInput.vertexBindingDescriptionCount = 1;
        pipelineVertexInput.pVertexBindingDescriptions = &vertexBindingDesc;
        info.pVertexInputState = &pipelineVertexInput;
    }
    info.pInputAssemblyState = &inputAssemblyState;
    if (0 == tesselationState.patchControlPoints)
        info.pTessellationState = nullptr;
    else
        info.pTessellationState = &tesselationState;
    info.pViewportState = &viewportState;
    info.pRasterizationState = &rasterizationState;
    info.pMultisampleState = &multisampleState;
    info.pDepthStencilState = &depthStencilState;
    info.pColorBlendState = &colorBlendState;
    VkPipelineDynamicStateCreateInfo dynamicState;
    if (0 == dynamicStates.size())
        info.pDynamicState = nullptr;
    else
    {
        dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.pNext = 0;
        dynamicState.flags = 0;
        dynamicState.dynamicStateCount = MAGMA_COUNT(dynamicStates);
        dynamicState.pDynamicStates = dynamicStates.begin();
        info.pDynamicState = &dynamicState;
    }
    if (layout)
        info.layout = *layout;
    else
    {
        defaultLayout = std::make_unique<PipelineLayout>(this->device);
        info.layout = *defaultLayout;
    }
    info.renderPass = *renderPass;
    info.subpass = subpass;
    info.basePipelineHandle = MAGMA_OPTIONAL_HANDLE(basePipeline);
    info.basePipelineIndex = -1;
    const VkResult create = vkCreateGraphicsPipelines(MAGMA_HANDLE(device), MAGMA_OPTIONAL_HANDLE(pipelineCache), 1, &info, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
    MAGMA_THROW_FAILURE(create, "failed to create graphics pipeline");
}

ComputePipeline::ComputePipeline(std::shared_ptr<Device> device, std::shared_ptr<const PipelineCache> pipelineCache,
    const PipelineShaderStage& stage,
    std::shared_ptr<const PipelineLayout> layout /* nullptr */,
    std::shared_ptr<const ComputePipeline> basePipeline /* nullptr */,
    VkPipelineCreateFlags flags /* 0 */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    Pipeline(std::move(device), std::move(allocator))
{
    VkComputePipelineCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = flags;
    if (basePipeline)
        info.flags |= VK_PIPELINE_CREATE_DERIVATIVE_BIT;
    info.stage = stage;
    if (layout)
        info.layout = *layout;
    else
    {
        defaultLayout = std::make_unique<PipelineLayout>(this->device);
        info.layout = *defaultLayout;
    }
    info.basePipelineHandle = MAGMA_OPTIONAL_HANDLE(basePipeline);
    info.basePipelineIndex = -1;
    const VkResult create = vkCreateComputePipelines(MAGMA_HANDLE(device), MAGMA_OPTIONAL_HANDLE(pipelineCache), 1, &info, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
    MAGMA_THROW_FAILURE(create, "failed to create compute pipeline");
}

RaytracingPipeline::RaytracingPipeline(std::shared_ptr<Device> device, std::shared_ptr<const PipelineCache> pipelineCache,
    const std::vector<PipelineShaderStage>& stages, const std::vector<RaytracingShaderGroup>& groups,
    uint32_t maxRecursionDepth, std::shared_ptr<const PipelineLayout> layout,
    std::shared_ptr<const RaytracingPipeline> basePipeline /* nullptr */,
    VkPipelineCreateFlags flags /* 0 */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    Pipeline(std::move(device), std::move(allocator))
{
    if (stages.empty())
        MAGMA_THROW("shader stages are empty");
    if (groups.empty())
        MAGMA_THROW("shader groups are empty");
    VkRayTracingPipelineCreateInfoNV info;
    info.sType = VK_STRUCTURE_TYPE_RAY_TRACING_PIPELINE_CREATE_INFO_NV;
    info.pNext = nullptr;
    info.flags = flags;
    if (basePipeline)
        info.flags |= VK_PIPELINE_CREATE_DERIVATIVE_BIT;
    MAGMA_STACK_ARRAY(VkPipelineShaderStageCreateInfo, dereferencedStages, stages.size());
    for (auto& stage : stages)
        dereferencedStages.put(stage);
    info.stageCount = MAGMA_COUNT(dereferencedStages);
    info.pStages = dereferencedStages;
    info.groupCount = MAGMA_COUNT(groups);
    info.pGroups = groups.data();
    info.maxRecursionDepth = maxRecursionDepth;
    if (layout)
        info.layout = *layout;
    else
    {
        defaultLayout = std::make_unique<PipelineLayout>(this->device);
        info.layout = *defaultLayout;
    }
    info.basePipelineHandle = MAGMA_OPTIONAL_HANDLE(basePipeline);
    info.basePipelineIndex = -1;
    MAGMA_DEVICE_EXTENSION(vkCreateRayTracingPipelinesNV, VK_NV_RAY_TRACING_EXTENSION_NAME);
    const VkResult create = vkCreateRayTracingPipelinesNV(MAGMA_HANDLE(device), MAGMA_OPTIONAL_HANDLE(pipelineCache), 1, &info, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
    MAGMA_THROW_FAILURE(create, "failed to create raytracing pipeline");
}

std::vector<VkShaderModule> RaytracingPipeline::getShaderGroupHandles(uint32_t firstGroup, uint32_t groupCount) const
{
    std::vector<VkShaderModule> shaders(groupCount);
    const size_t dataSize = sizeof(VkShaderModule) * groupCount;
    MAGMA_DEVICE_EXTENSION(vkGetRayTracingShaderGroupHandlesNV, VK_NV_RAY_TRACING_EXTENSION_NAME);
    const VkResult get = vkGetRayTracingShaderGroupHandlesNV(MAGMA_HANDLE(device), handle, firstGroup, groupCount, dataSize, shaders.data());
    MAGMA_THROW_FAILURE(get, "failed to get raytracing shader handles");
    return shaders;
}

void RaytracingPipeline::compileDeferred(uint32_t shaderIndex)
{
    MAGMA_DEVICE_EXTENSION(vkCompileDeferredNV, VK_NV_RAY_TRACING_EXTENSION_NAME);
    const VkResult compile = vkCompileDeferredNV(MAGMA_HANDLE(device), handle, shaderIndex);
    MAGMA_THROW_FAILURE(compile, "failed to compile shader deferred");
}
} // namespace magma
