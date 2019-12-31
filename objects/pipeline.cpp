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
#include "pch.h"
#pragma hdrstop
#include "pipeline.h"
#include "device.h"
#include "../allocator/allocator.h"
#include "../misc/deviceExtension.h"
#include "../misc/exception.h"

namespace magma
{
Pipeline::Pipeline(std::shared_ptr<Device> device, std::shared_ptr<PipelineLayout> layout,
    std::shared_ptr<Pipeline> basePipeline, std::shared_ptr<PipelineCache> cache, std::shared_ptr<IAllocator> allocator):
    NonDispatchable<VkPipeline>(VK_OBJECT_TYPE_PIPELINE, std::move(device), std::move(allocator)),
    layout(std::move(layout)),
    basePipeline(std::move(basePipeline)),
    cache(std::move(cache))
{
    if (!this->layout)
    {   // Layout must be a valid VkPipelineLayout handle
        this->layout = std::make_shared<PipelineLayout>(this->device);
    }
}

Pipeline::~Pipeline()
{
    vkDestroyPipeline(MAGMA_HANDLE(device), handle, MAGMA_OPTIONAL_INSTANCE(allocator));
}

VkShaderStatisticsInfoAMD Pipeline::getShaderStatistics(VkShaderStageFlagBits stage) const
{
    MAGMA_DEVICE_EXTENSION(vkGetShaderInfoAMD, VK_AMD_SHADER_INFO_EXTENSION_NAME);
    std::size_t infoSize = sizeof(VkShaderStatisticsInfoAMD);
    VkShaderStatisticsInfoAMD info;
    const VkResult getStatistics = vkGetShaderInfoAMD(MAGMA_HANDLE(device), handle, stage, VK_SHADER_INFO_TYPE_STATISTICS_AMD, &infoSize, &info);
    MAGMA_THROW_FAILURE(getStatistics, "failed to get shader statistics");
    return info;
}

std::vector<uint8_t> Pipeline::getShaderBinary(VkShaderStageFlagBits stage) const
{
    MAGMA_DEVICE_EXTENSION(vkGetShaderInfoAMD, VK_AMD_SHADER_INFO_EXTENSION_NAME);
    std::size_t binarySize;
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
    std::size_t disassemblySize;
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
} // namespace magma
