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
#include "rayTracingShaderGroup.h"

namespace magma
{
RayTracingShaderGroup::RayTracingShaderGroup(VkRayTracingShaderGroupTypeNV type,
    uint32_t generalShader, uint32_t closestHitShader, uint32_t anyHitShader, uint32_t intersectionShader) noexcept
{
    sType = VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_NV;
    pNext = nullptr;
    this->type = type;
    this->generalShader = generalShader;
    this->closestHitShader = closestHitShader;
    this->anyHitShader = anyHitShader;
    this->intersectionShader = intersectionShader;
}

std::size_t RayTracingShaderGroup::hash() const noexcept
{
    return detail::hashArgs(
        sType,
        type,
        generalShader,
        closestHitShader,
        anyHitShader,
        intersectionShader);
}
} // namespace magma
