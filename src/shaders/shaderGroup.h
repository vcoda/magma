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

namespace magma
{
    /* The embedded parameters are accessed in the shader through
       a special buffer type declared with the shaderRecordEXT layout.
       The "shaderRecordEXT" qualifier is used to declare a buffer
       block and represents a buffer within a shader record. It is
       supported only in ray tracing stages.

       Usage example:

       layout(shaderRecordEXT) buffer block
       {
           vec2 blendWeight;
       }; */

#ifdef VK_KHR_ray_tracing_pipeline
    struct ShaderRecord
    {
        ShaderRecord() noexcept = default;
        template<class Block>
        ShaderRecord(const Block& block):
            embedded(sizeof(Block))
        {
            static_assert(std::is_trivially_copyable<Block>::value,
                "embedded data must be trivially copyable");
            memcpy(embedded.data(), &block, sizeof(Block));
        }

        std::vector<uint8_t> embedded;
    };

    class Buffer;
    class ShaderBindingTable;

    /* Ray tracing pipelines can contain multiple shader groups
       that can be bound individually. The primary purpose of
       shader groups is allowing the device to bind different
       pipeline state during vkCmdTraceRays* execution.
       There are several ray tracing shader groups types:

        * General
        * Triangles Hit
        * Procedural Hit

       Shader group is a collection of opaque shader handles
       packed with optional embedded data. All data must be
       aligned according to alignment properties described by
       VkPhysicalDeviceRayTracingPipelinePropertiesKHR structure. */

    class ShaderGroup : NonCopyable
    {
    public:
        ShaderGroup() noexcept;
        void addGroupIndex(uint32_t index) { indices.push_back(index); }
        void calculateStride(const VkPhysicalDeviceRayTracingPipelinePropertiesKHR properties) noexcept;
        std::vector<uint8_t> getBindingTableData(const std::vector<uint8_t>& shaderGroupHandles,
            const VkPhysicalDeviceRayTracingPipelinePropertiesKHR properties);
        VkStridedDeviceAddressRegionKHR getRegion(uint32_t indexOffset) const noexcept;

    private:
        std::vector<uint32_t> indices;
        std::unordered_map<uint32_t, ShaderRecord> shaderRecords;
        std::unique_ptr<Buffer> shaderBindingTable;
        uint32_t stride;
        bool raygen;
        friend ShaderBindingTable;
    };
#endif // VK_KHR_ray_tracing_pipeline
} // namespace magma
