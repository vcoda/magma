/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2026 Victor Coda.

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
namespace magma::packed
{
inline void dwordToFloat4(uint32_t packed, float& r, float& g, float& b, float& a) noexcept
{
    constexpr float scale = 1.f/0xFF;
    r = ((packed >> 24) & 0xFF) * scale;
    g = ((packed >> 16) & 0xFF) * scale;
    b = ((packed >> 8) & 0xFF) * scale;
    a = (packed & 0xFF) * scale;
}
} // namespace magma::packed
