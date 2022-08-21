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
#include "rayTracingBuffer.h"

#if defined(VK_NV_ray_tracing) && !defined(VK_SHADER_UNUSED_KHR)

/* With an early SDK definitions weren't provided in the headers
   so I declared my own structures that obey vendor layout. */

typedef struct VkTransformMatrixNV {
    float matrix[3][4];
} VkTransformMatrixNV;

typedef struct VkAccelerationStructureInstanceNV {
    VkTransformMatrixNV transform;
    uint32_t instanceCustomIndex:24;
    uint32_t mask:8;
    uint32_t instanceShaderBindingTableRecordOffset:24;
    VkFlags flags:8;
    uint64_t accelerationStructureReference;
} VkAccelerationStructureInstanceNV;

#endif // VK_NV_ray_tracing && !VK_SHADER_UNUSED_KHR

namespace magma
{
    class AccelerationStructure;
    class CommandBuffer;
    class SrcTransferBuffer;

#ifdef VK_NV_ray_tracing
    typedef VkTransformMatrixNV TransformMatrix;

    /* Single acceleration structure instance for building into an acceleration structure geometry. */

    class AccelerationStructureInstance : public VkAccelerationStructureInstanceNV
    {
    public:
        AccelerationStructureInstance() noexcept;
        void setTransform(const TransformMatrix& transform_) noexcept { transform = transform_; }
        void setInstanceIndex(uint32_t instanceIndex) noexcept { instanceCustomIndex = instanceIndex; }
        void setVisibilityMask(uint8_t mask_) noexcept { mask = mask_; }
        void setShaderBindingTableOffset(uint32_t offset) noexcept { instanceShaderBindingTableRecordOffset = offset; }
        void setCullTriangles(bool enable) noexcept { setFlag(VK_GEOMETRY_INSTANCE_TRIANGLE_CULL_DISABLE_BIT_NV, !enable); }
        bool cullEnabled() const noexcept { return !(flags & VK_GEOMETRY_INSTANCE_TRIANGLE_CULL_DISABLE_BIT_NV); }
        void setFrontTriangle(bool ccw) noexcept { setFlag(VK_GEOMETRY_INSTANCE_TRIANGLE_FRONT_COUNTERCLOCKWISE_BIT_NV, ccw); }
        bool frontTriangleCCw() const noexcept { return flags & VK_GEOMETRY_INSTANCE_TRIANGLE_FRONT_COUNTERCLOCKWISE_BIT_NV; }
        void setForceOpaque(bool opaque) noexcept { setFlag(VK_GEOMETRY_INSTANCE_FORCE_OPAQUE_BIT_NV, opaque); }
        void setForceNoOpaque(bool noOpaque) noexcept { setFlag(VK_GEOMETRY_INSTANCE_FORCE_NO_OPAQUE_BIT_NV, noOpaque); }
        bool opaque() const noexcept { return flags & VK_GEOMETRY_INSTANCE_FORCE_OPAQUE_BIT_NV; }
        bool noOpaque() const noexcept { return flags & VK_GEOMETRY_INSTANCE_FORCE_NO_OPAQUE_BIT_NV; }
        void setAccelerationStructure(std::shared_ptr<const AccelerationStructure> accelerationStructure);

    private:
        void setFlag(VkGeometryInstanceFlagBitsKHR bit, bool set) noexcept { if (set) flags |= bit; else flags &= ~bit; }
    };

    /* Buffer containing an array of VkAccelerationStructureInstanceKHR structures
       defining acceleration structures. */

    class AccelerationStructureInstanceBuffer : public RayTracingBuffer
    {
    public:
        explicit AccelerationStructureInstanceBuffer(std::shared_ptr<Device> device,
            uint32_t instanceCount,
            std::shared_ptr<Allocator> allocator = nullptr,
            VkBufferCreateFlags flags = 0,
            bool persistentlyMapped = false,
            float memoryPriority = MAGMA_MEMORY_PRIORITY,
            const Sharing& sharing = Sharing());
        ~AccelerationStructureInstanceBuffer();
        uint32_t getInstanceCount() const noexcept { return instanceCount; }
        AccelerationStructureInstance& getInstance(uint32_t instanceIndex) noexcept { return instances[instanceIndex]; }
        const AccelerationStructureInstance& getInstance(uint32_t instanceIndex) const noexcept { return instances[instanceIndex]; }
        bool persistentlyMapped() const noexcept { return persistent; }
        bool update(std::shared_ptr<CommandBuffer> cmdBuffer,
            uint32_t firstInstance,
            uint32_t instanceCount);

    private:
        const uint32_t instanceCount;
        const bool persistent;
        std::shared_ptr<SrcTransferBuffer> stagingBuffer;
        std::vector<AccelerationStructureInstance> instanceArray;
        AccelerationStructureInstance *instances;
    };
#endif // VK_NV_ray_tracing
} // namespace magma
