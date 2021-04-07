/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2020 Victor Coda.

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
#include "rayTracingBuffer.h"

namespace magma
{
    class AccelerationStructure;
    class CommandBuffer;
    class SrcTransferBuffer;

#ifdef VK_NV_ray_tracing
    typedef VkTransformMatrixKHR TransformMatrix;

    /* Single acceleration structure instance for building into an acceleration structure geometry. */

    class AccelerationStructureInstance : public VkAccelerationStructureInstanceKHR
    {
    public:
        AccelerationStructureInstance() noexcept;
        void setTransform(const TransformMatrix& transform) noexcept { this->transform = transform; }
        void setCustomIndex(uint32_t instanceIndex) noexcept { instanceCustomIndex = instanceIndex; }
        void setVisibilityMask(uint8_t mask) noexcept { this->mask = mask; }
        void setShaderBindingTableOffset(uint32_t offset) noexcept { instanceShaderBindingTableRecordOffset = offset; }
        void enableTriangleCull() noexcept { flags &= ~VK_GEOMETRY_INSTANCE_TRIANGLE_CULL_DISABLE_BIT_NV; }
        void disableTriangleCull() noexcept { flags |= VK_GEOMETRY_INSTANCE_TRIANGLE_CULL_DISABLE_BIT_NV; }
        bool triangleCullEnabled() const noexcept { return flags & VK_GEOMETRY_INSTANCE_TRIANGLE_CULL_DISABLE_BIT_NV; }
        void setTriangleFrontCW() noexcept { flags &= ~VK_GEOMETRY_INSTANCE_TRIANGLE_FRONT_COUNTERCLOCKWISE_BIT_NV; }
        void setTriangleFrontCCW() noexcept { flags |= VK_GEOMETRY_INSTANCE_TRIANGLE_FRONT_COUNTERCLOCKWISE_BIT_NV; }
        bool triangleFrontCCW() const noexcept { return flags & VK_GEOMETRY_INSTANCE_TRIANGLE_FRONT_COUNTERCLOCKWISE_BIT_NV; }
        void forceGeometryOpaque(bool opaque) noexcept { if (opaque) flags |= VK_GEOMETRY_INSTANCE_FORCE_OPAQUE_BIT_NV; 
                                                         else flags &= ~VK_GEOMETRY_INSTANCE_FORCE_OPAQUE_BIT_NV; }
        bool geometryForcedOpaque() const noexcept { return flags & VK_GEOMETRY_INSTANCE_FORCE_OPAQUE_BIT_NV; }
        void forceGeometryNoOpaque(bool noOpaque) noexcept { if (noOpaque) flags |= VK_GEOMETRY_INSTANCE_FORCE_NO_OPAQUE_BIT_NV;
                                                           else flags &= ~VK_GEOMETRY_INSTANCE_FORCE_NO_OPAQUE_BIT_NV; }
        bool geometryForcedNoOpaque() const noexcept { return flags & VK_GEOMETRY_INSTANCE_FORCE_NO_OPAQUE_BIT_NV; }
        void setAccelerationStructure(std::shared_ptr<AccelerationStructure> accelerationStructure);
    };

    /* Buffer containing an array of VkAccelerationStructureInstanceKHR structures 
       defining acceleration structures. */

    class AccelerationStructureInstanceBuffer : public RayTracingBuffer
    {
    public:
        explicit AccelerationStructureInstanceBuffer(std::shared_ptr<Device> device,
            uint32_t instanceCount,
            VkBufferCreateFlags flags = 0,
            const Sharing& sharing = Sharing(),
            std::shared_ptr<IAllocator> allocator = nullptr,
            CopyMemoryFunction copyFn = nullptr);
        uint32_t getInstanceCount() const noexcept { return (uint32_t)instances.size(); }
        AccelerationStructureInstance& getInstance(uint32_t instanceIndex) noexcept { return instances[instanceIndex]; }
        bool update(std::shared_ptr<CommandBuffer> cmdBuffer,
            uint32_t firstInstance, 
            uint32_t instanceCount);

    private:
        std::shared_ptr<SrcTransferBuffer> stagingBuffer;
        std::vector<AccelerationStructureInstance> instances;
    };
#endif // VK_NV_ray_tracing
} // namespace magma
