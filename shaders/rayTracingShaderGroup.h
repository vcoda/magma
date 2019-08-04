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
    class RayTracingShaderGroup : public VkRayTracingShaderGroupCreateInfoNV
    {
    protected:
        explicit RayTracingShaderGroup(VkRayTracingShaderGroupTypeNV type,
            uint32_t generalShader,
            uint32_t closestHitShader,
            uint32_t anyHitShader,
            uint32_t intersectionShader);
    public:
        size_t hash() const noexcept;
    };

    // TODO: reconsider

    class GeneralRayTracingShaderGroup : public RayTracingShaderGroup
    {
    public:
        explicit GeneralRayTracingShaderGroup(uint32_t generalShader):
            RayTracingShaderGroup(VK_RAY_TRACING_SHADER_GROUP_TYPE_GENERAL_NV, generalShader, VK_SHADER_UNUSED_NV, VK_SHADER_UNUSED_NV, VK_SHADER_UNUSED_NV) {}
    };

    class TrianglesHitRayTracingShaderGroup : public RayTracingShaderGroup
    {
    public:
        explicit TrianglesHitRayTracingShaderGroup(uint32_t closestHitShader, uint32_t anyHitShader):
            RayTracingShaderGroup(VK_RAY_TRACING_SHADER_GROUP_TYPE_TRIANGLES_HIT_GROUP_NV, VK_SHADER_UNUSED_NV, closestHitShader, anyHitShader, VK_SHADER_UNUSED_NV) {}
    };

    class ProceduralHitRayTracingShaderGroup : public RayTracingShaderGroup
    {
    public:
        explicit ProceduralHitRayTracingShaderGroup(uint32_t intersectionShader):
            RayTracingShaderGroup(VK_RAY_TRACING_SHADER_GROUP_TYPE_PROCEDURAL_HIT_GROUP_NV, VK_SHADER_UNUSED_NV, VK_SHADER_UNUSED_NV, VK_SHADER_UNUSED_NV, intersectionShader) {}
    };
} // namespace magma
