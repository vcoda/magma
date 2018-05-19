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
namespace magma
{
inline Scissor::Scissor(int32_t x, int32_t y, const VkExtent2D& extent)
{
    offset.x = x;
    offset.y = y;
    this->extent = extent;
}

inline Scissor::Scissor(float x, float y, const VkExtent2D& extent)
{
    offset.x = static_cast<int32_t>(x);
    offset.y = static_cast<int32_t>(y);
    this->extent = extent;
}

inline Scissor::Scissor(int32_t x, int32_t y, uint32_t width, uint32_t height)
{
    offset.x = x;
    offset.y = y;
    extent.width = width;
    extent.height = height;
}

inline Scissor::Scissor(float x, float y, float width, float height)
{
    offset.x = static_cast<int32_t>(x);
    offset.y = static_cast<int32_t>(y);
    extent.width = static_cast<uint32_t>(width);
    extent.height = static_cast<uint32_t>(height);
}
} // namespace magma