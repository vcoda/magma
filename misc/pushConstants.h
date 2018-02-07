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
#include "../vulkan.h"

namespace magma
{
    namespace pushconstants
    {
        template<typename Type>
        struct PushConstantRange : VkPushConstantRange
        {
            PushConstantRange(VkShaderStageFlags flags, uint32_t rangeOffset = 0)
            {
                stageFlags = flags;
                offset = rangeOffset;
                size = sizeof(Type);
            }
        };

        template<typename Type>
        struct VertexConstantRange : PushConstantRange<Type>
        {
            VertexConstantRange(uint32_t offset = 0): PushConstantRange(VK_SHADER_STAGE_VERTEX_BIT, offset) {}
        };

        template<typename Type>
        struct VertexGeometryConstantRange : PushConstantRange<Type>
        {
            VertexGeometryConstantRange(uint32_t offset = 0): PushConstantRange(VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_GEOMETRY_BIT, offset) {}
        };

        template<typename Type>
        struct VertexFragmentConstantRange : PushConstantRange<Type>
        {
            VertexFragmentConstantRange(uint32_t offset = 0): PushConstantRange(VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, offset) {}
        };

        template<typename Type>
        struct ComputeConstantRange : PushConstantRange<Type>
        {
            ComputeConstantRange(uint32_t offset = 0): PushConstantRange(VK_SHADER_STAGE_COMPUTE_BIT, offset) {}
        };

        template<typename Type>
        struct GraphicsConstantRange : PushConstantRange<Type>
        {
            GraphicsConstantRange(uint32_t offset = 0): PushConstantRange(VK_SHADER_STAGE_ALL_GRAPHICS, offset) {}
        };

        template<typename Type>
        struct PipelineConstantRange : PushConstantRange<Type>
        {
            PipelineConstantRange(uint32_t offset = 0): PushConstantRange(VK_SHADER_STAGE_ALL, offset) {}
        };
    } // namespace pushconstants
} // namespace magma
