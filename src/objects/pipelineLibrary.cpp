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
#include "pipelineLibrary.h"
#include "pipelineLayout.h"
#include "device.h"
#include "../shaders/pipelineShaderStage.h"
#include "../states/vertexInputState.h"
#include "../states/inputAssemblyState.h"
#include "../allocator/allocator.h"
#include "../exceptions/errorResult.h"

namespace magma
{
#ifdef VK_KHR_pipeline_library
PipelineLibrary::PipelineLibrary(std::shared_ptr<Device> device,
    std::shared_ptr<IAllocator> hostAllocator /* nullptr */) noexcept:
    device(std::move(device)),
    hostAllocator(std::move(hostAllocator))
{}

PipelineLibrary::~PipelineLibrary()
{
    for (auto pipeline: libraries)
        vkDestroyPipeline(MAGMA_HANDLE(device), pipeline, MAGMA_OPTIONAL_INSTANCE(hostAllocator));
}

#ifdef VK_EXT_graphics_pipeline_library
void PipelineLibrary::compileVertexInputInterface(const VertexInputState& vertexInputState, const InputAssemblyState& inputAssemblyState)
{
    VkGraphicsPipelineCreateInfo graphicsPipelineInfo = {};
    VkGraphicsPipelineLibraryCreateInfoEXT graphicsPipelineLibraryInfo;
    graphicsPipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    graphicsPipelineInfo.pNext = &graphicsPipelineLibraryInfo;
    graphicsPipelineInfo.flags = VK_PIPELINE_CREATE_LIBRARY_BIT_KHR;
    graphicsPipelineInfo.pVertexInputState = &vertexInputState;
    graphicsPipelineInfo.pInputAssemblyState = &inputAssemblyState;
    graphicsPipelineInfo.basePipelineIndex = -1;
    graphicsPipelineLibraryInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_LIBRARY_CREATE_INFO_EXT;
    graphicsPipelineLibraryInfo.pNext = nullptr;
    graphicsPipelineLibraryInfo.flags = VK_GRAPHICS_PIPELINE_LIBRARY_VERTEX_INPUT_INTERFACE_BIT_EXT;
    VkPipeline handle = 0;
    const VkResult result = vkCreateGraphicsPipelines(MAGMA_HANDLE(device), VK_NULL_HANDLE,
        1, &graphicsPipelineInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to compile vertex input interface subset");
    libraries.push_back(handle);
}
#endif // VK_EXT_graphics_pipeline_library
#endif // VK_KHR_pipeline_library
} // namespace magma
