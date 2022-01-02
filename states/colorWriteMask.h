/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2022 Victor Coda.

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
    namespace colorwritemask
    {
        constexpr VkColorComponentFlags none(0);

        constexpr VkColorComponentFlags r(VK_COLOR_COMPONENT_R_BIT);
        constexpr VkColorComponentFlags g(VK_COLOR_COMPONENT_G_BIT);
        constexpr VkColorComponentFlags b(VK_COLOR_COMPONENT_B_BIT);
        constexpr VkColorComponentFlags a(VK_COLOR_COMPONENT_A_BIT);

        constexpr VkColorComponentFlags rg(VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT);
        constexpr VkColorComponentFlags rb(VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_B_BIT);
        constexpr VkColorComponentFlags ra(VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_A_BIT);
        constexpr VkColorComponentFlags gb(VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT);
        constexpr VkColorComponentFlags ga(VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_A_BIT);
        constexpr VkColorComponentFlags ba(VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT);

        constexpr VkColorComponentFlags rgb(VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT);
        constexpr VkColorComponentFlags rga(VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_A_BIT);
        constexpr VkColorComponentFlags rba(VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT);
        constexpr VkColorComponentFlags gba(VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT);

        constexpr VkColorComponentFlags rgba(VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT);
    } // namespace colorwritemask
} // namespace magma
