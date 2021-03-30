/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2021 Victor Coda.

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
#include "shaderBindingTable.h"
#include "../objects/device.h"
#include "../objects/physicalDevice.h"
#include "../objects/deviceMemory.h"

namespace magma
{
#ifdef VK_NV_ray_tracing
ShaderBindingTable::ShaderBindingTable(std::shared_ptr<Device> device, const void *shaderGroupHandles, uint32_t groupCount):
    SrcTransferBuffer(device, device->getPhysicalDevice()->getRayTracingProperties().shaderGroupHandleSize * groupCount) // TODO: VK_BUFFER_USAGE_SHADER_BINDING_TABLE_BIT_KHR
{
    uint8_t *shaderBindingData = getMemory()->map<uint8_t>();
    if (shaderBindingData)
    {
        const VkPhysicalDeviceRayTracingPropertiesNV& rayTracingProperties = device->getPhysicalDevice()->getRayTracingProperties();
        const uint32_t handleSize = rayTracingProperties.shaderGroupHandleSize;
        const uint32_t baseAlignment = rayTracingProperties.shaderGroupBaseAlignment;
        for (uint32_t groupIndex = 0; groupIndex < groupCount; ++groupIndex)
        {
            memcpy(shaderBindingData, (const uint8_t *)shaderGroupHandles + groupIndex * handleSize, handleSize);
            shaderBindingData += baseAlignment;
        }
        getMemory()->unmap();
    }
}

ShaderBindingTable::ShaderBindingTable(std::shared_ptr<Device> device, const std::vector<uint8_t>& shaderGroupHandles, uint32_t groupCount):
    ShaderBindingTable(std::move(device), shaderGroupHandles.data(), groupCount) 
{}
#endif // VK_NV_ray_tracing
} // namespace magma
