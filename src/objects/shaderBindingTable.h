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
#include "buffer.h"

namespace magma
{
    /* A shader binding table is a resource which establishes
       the relationship between the ray tracing pipeline and
       the acceleration structures that were built for the
       ray tracing pipeline. */

#ifdef VK_KHR_ray_tracing_pipeline
    class ShaderBindingTable : public Buffer
    {
    public:
        explicit ShaderBindingTable(std::shared_ptr<CommandBuffer> cmdBuffer,
            const void *shaderGroupHandles,
            size_t shaderGroupSize,
            const VkShaderStageFlagBits *shaderStageFlags,
            std::shared_ptr<Allocator> allocator = nullptr,
            const Initializer& optional = Initializer(),
            const Sharing& sharing = Sharing());
        explicit ShaderBindingTable(std::shared_ptr<CommandBuffer> cmdBuffer,
            const std::vector<uint8_t>& shaderGroupHandles,
            const std::vector<VkShaderStageFlagBits>& shaderStageFlags,
            std::shared_ptr<Allocator> allocator = nullptr,
            const Initializer& optional = Initializer(),
            const Sharing& sharing = Sharing());
        const VkStridedDeviceAddressRegionKHR *getRaygenStage() const noexcept { return &raygen; }
        const VkStridedDeviceAddressRegionKHR *getMissStage() const noexcept { return &miss; }
        const VkStridedDeviceAddressRegionKHR *getHitStage() const noexcept { return &hit; }
        const VkStridedDeviceAddressRegionKHR *getCallableStage() const noexcept { return &callable; }

    private:
        static VkDeviceSize calculateAlignedSize(const std::shared_ptr<Device>& device,
            size_t dataSize) noexcept;
        void setStageData(VkShaderStageFlagBits stage, uint32_t stageIndex,
            VkDeviceAddress baseAddress,
            uint32_t shaderGroupHandleSize,
            uint32_t shaderGroupBaseAlignment) noexcept;

        VkStridedDeviceAddressRegionKHR raygen = {};
        VkStridedDeviceAddressRegionKHR miss = {};
        VkStridedDeviceAddressRegionKHR hit = {};
        VkStridedDeviceAddressRegionKHR callable = {};
    };
#endif // VK_KHR_ray_tracing_pipeline
} // namespace magma
