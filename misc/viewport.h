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

namespace magma
{
    struct Viewport : VkViewport
    {
        Viewport(float x, float y, float width, float height,
            float minDepth = 0.f, float maxDepth = 1.f)
        {
            this->x = x;
            this->y = y;
            this->width = width;
            this->height = height;
            this->minDepth = minDepth;
            this->maxDepth = maxDepth;
        }    

        Viewport(float x, float y, const VkExtent2D& extent,
            float minDepth = 0.f, float maxDepth = 1.f)
        {
            this->x = x;
            this->y = y;
            width = static_cast<float>(extent.width);
            height = static_cast<float>(extent.height);
            this->minDepth = minDepth;
            this->maxDepth = maxDepth;
        }

        Viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height,
            float minDepth = 0.f, float maxDepth = 1.f)
        {
            this->x = static_cast<float>(x);
            this->y = static_cast<float>(y);
            this->width = static_cast<float>(width);
            this->height = static_cast<float>(height);
            this->minDepth = minDepth;
            this->maxDepth = maxDepth;
        }
    };
} // namespace magma