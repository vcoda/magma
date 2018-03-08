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
#pragma once
#include "../vulkan.h"

namespace magma
{
    const char *stringize(VkFormat format);
    const char *stringize(VkImageType param);
    const char *stringize(VkImageTiling param);
    const char *stringize(VkPhysicalDeviceType param);
    const char *stringize(VkQueryType param);

    const char *stringize(VkPipelineStageFlagBits bit);
    const char *stringize(VkImageUsageFlagBits bit);
    const char *stringize(VkSurfaceTransformFlagBitsKHR bit);
    const char *stringize(VkCompositeAlphaFlagBitsKHR bit);
} // namespace magma
