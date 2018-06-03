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
#include "image1DArray.h"

namespace magma
{
Image1DArray::Image1DArray(std::shared_ptr<const Device> device, VkFormat format,
    uint32_t width, uint32_t mipLevels, uint32_t arrayLayers, VkImageUsageFlags usage,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    Image(std::move(device), VK_IMAGE_TYPE_1D, format, VkExtent3D{width, 1, 1}, mipLevels, arrayLayers, 1, usage, 0, std::move(allocator))
{}
} // namespace magma
