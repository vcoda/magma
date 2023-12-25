/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2023 Victor Coda.

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
    /* Ray tracing pipelines can contain multiple shader groups
       that can be bound individually. Each shader group behaves
       as if it was a pipeline using the shader group's state. */

#ifdef VK_NV_ray_tracing
    class RayTracingShaderGroup : public VkRayTracingShaderGroupCreateInfoNV
    {
    protected:
        explicit RayTracingShaderGroup(VkRayTracingShaderGroupTypeNV type,
            uint32_t generalShader,
            uint32_t closestHitShader,
            uint32_t anyHitShader,
            uint32_t intersectionShader) noexcept;

    public:
        hash_t hash() const noexcept;
    };

    /* General shader is the index of the ray generation, miss, or callable shader. */

    class GeneralRayTracingShaderGroup : public RayTracingShaderGroup
    {
    public:
        explicit GeneralRayTracingShaderGroup(uint32_t generalShader) noexcept:
            RayTracingShaderGroup(VK_RAY_TRACING_SHADER_GROUP_TYPE_GENERAL_NV, generalShader, VK_SHADER_UNUSED_NV, VK_SHADER_UNUSED_NV, VK_SHADER_UNUSED_NV) {}
    };

    /* Triangle hit shader is the optional index of the closest hit or any hit shader(s). */

    class TrianglesHitRayTracingShaderGroup : public RayTracingShaderGroup
    {
    public:
        explicit TrianglesHitRayTracingShaderGroup(uint32_t closestHitShader, uint32_t anyHitShader = VK_SHADER_UNUSED_NV) noexcept:
            RayTracingShaderGroup(VK_RAY_TRACING_SHADER_GROUP_TYPE_TRIANGLES_HIT_GROUP_NV, VK_SHADER_UNUSED_NV, closestHitShader, anyHitShader, VK_SHADER_UNUSED_NV) {}
    };

    /* Procedural hit shader is the optional index of the intersection shader. */

    class ProceduralHitRayTracingShaderGroup : public RayTracingShaderGroup
    {
    public:
        explicit ProceduralHitRayTracingShaderGroup(uint32_t intersectionShader) noexcept:
            RayTracingShaderGroup(VK_RAY_TRACING_SHADER_GROUP_TYPE_PROCEDURAL_HIT_GROUP_NV, VK_SHADER_UNUSED_NV, VK_SHADER_UNUSED_NV, VK_SHADER_UNUSED_NV, intersectionShader) {}
    };
#endif // VK_NV_ray_tracing
} // namespace magma
