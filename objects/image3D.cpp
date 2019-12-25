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
#include "image3D.h"

namespace magma
{
Image3D::Image3D(std::shared_ptr<Device> device, VkFormat format, const VkExtent3D& extent, VkImageUsageFlags usage,
    const Sharing& sharing /* default */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    Image(std::move(device), VK_IMAGE_TYPE_3D, format, extent,
        1, // mipLevels,
        1, // arrayLayers
        1, // samples
        VK_IMAGE_TILING_OPTIMAL,
        usage,
        0, // flags
        sharing,
        std::move(allocator))
{}
} // namespace magma
