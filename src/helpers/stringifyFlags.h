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
#pragma once

namespace magma
{
    namespace helpers
    {
        std::string stringifyMask(uint32_t mask);
        std::string stringifyImageFlags(VkImageCreateFlags flags);
        std::string stringifyImageUsage(VkImageUsageFlags flags);
        std::string stringifyBufferFlags(VkBufferCreateFlags flags);
        std::string stringifyBufferUsage(VkBufferUsageFlags flags);
        std::string stringifyPipelineShaderStageFlags(VkPipelineShaderStageCreateFlags flags);
        std::string stringifyShaderStageFlags(VkShaderStageFlags flags);
        std::string stringifyColorMask(VkColorComponentFlags flags);
    #ifdef VK_KHR_swapchain
        std::string stringifySwapchainFlags(VkSwapchainCreateFlagsKHR flags);
    #endif
    #ifdef VK_KHR_device_group
        std::string stringifyDeviceGroupPresentMode(VkDeviceGroupPresentModeFlagsKHR flags);
    #endif
    } // namespace helpers
} // namespace magma
