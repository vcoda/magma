/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2024 Victor Coda.

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
#include "pipelineShaderStage.h"
#include "rayTracingShaderGroup.h"
#include "shaderGroup.h"

namespace magma
{
    class RayTracingPipeline;
    class CommandBuffer;
    class Allocator;

    /* A shader binding table is a resource which establishes
       the relationship between the ray tracing pipeline and
       the acceleration structures that were built for the ray
       tracing pipeline. It indicates the shaders that operate
       on each geometry in an acceleration structure. In
       addition, it contains the resources accessed by each
       shader, including indices of textures, buffer device
       addresses, and constants.

       Each entry in the shader binding table consists of
       shaderGroupHandleSize bytes of data. The remainder of the
       data specified by the stride is application-visible data
       that can be referenced by a ShaderRecordBufferKHR block
       in the shader. */

#ifdef VK_KHR_ray_tracing_pipeline
    class ShaderBindingTable : NonCopyable
    {
    public:
        ShaderBindingTable() = default;
        explicit ShaderBindingTable(lent_ptr<const RayTracingPipeline> pipeline,
            lent_ptr<CommandBuffer> cmdBuffer,
            std::shared_ptr<Allocator> allocator = nullptr);
        uint32_t getShaderGroupCount() const noexcept { return core::countof(groups); }
        template<class Block>
        void addShaderRecord(VkShaderStageFlagBits stage,
            uint32_t groupIndex,
            const Block& block);
        void build(lent_ptr<const RayTracingPipeline> pipeline,
            lent_ptr<CommandBuffer> cmdBuffer,
            std::shared_ptr<Allocator> allocator = nullptr,
            const std::vector<VkRayTracingPipelineCreateInfoKHR>& librariesInfo = {});
        VkStridedDeviceAddressRegionKHR getDeviceAddressRegion(VkShaderStageFlagBits stage,
            uint32_t indexOffset = 0) const noexcept;

    private:
        void setupIndices(const std::vector<PipelineShaderStage>& shaderStages,
            const std::vector<RayTracingShaderGroup>& shaderGroups);

        std::unordered_map<VkShaderStageFlagBits, ShaderGroup> groups;
    };
#endif // VK_KHR_ray_tracing_pipeline
} // namespace magma

#include "shaderBindingTable.inl"
