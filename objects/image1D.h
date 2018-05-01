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
#include "image.h"

namespace magma
{
    class Image1D : public Image
    {
    public:
        Image1D(std::shared_ptr<const Device> device,
            VkFormat format,
            uint32_t width,
            uint32_t mipLevels,
            VkImageUsageFlags usage,
            std::shared_ptr<IAllocator> allocator = nullptr);
    };
} // namespace magma
