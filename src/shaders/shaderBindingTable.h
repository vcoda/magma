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
#include "../objects/buffer.h"

namespace magma
{
    class RayTracingPipeline;

    /* A shader binding table is a resource which establishes
       the relationship between the ray tracing pipeline and
       the acceleration structures that were built for the
       ray tracing pipeline. */

#ifdef VK_NV_ray_tracing
    class ShaderBindingTable : public Buffer
    {
    public:
        explicit ShaderBindingTable(std::shared_ptr<Device> device,
            const void *shaderGroupHandles,
            uint32_t groupCount,
            std::shared_ptr<Allocator> allocator = nullptr,
            const Initializer& optional = Initializer(),
            const Sharing& sharing = Sharing());
        explicit ShaderBindingTable(std::shared_ptr<Device> device,
            const std::vector<uint8_t>& shaderGroupHandles,
            uint32_t groupCount,
            std::shared_ptr<Allocator> allocator = nullptr,
            const Initializer& optional = Initializer(),
            const Sharing& sharing = Sharing());
        explicit ShaderBindingTable(std::shared_ptr<const RayTracingPipeline> pipeline,
            std::shared_ptr<Allocator> allocator = nullptr,
            const Initializer& optional = Initializer(),
            const Sharing& sharing = Sharing());
    };
#endif // VK_NV_ray_tracing
} // namespace magma
