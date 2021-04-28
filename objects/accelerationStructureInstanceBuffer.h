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

#if defined(VK_NV_ray_tracing) && !defined(VK_SHADER_UNUSED_KHR)

/* With early SDK definition wasn't provided in the headers 
   so I declare own structs which matches the specified layout. */

typedef struct VkTransformMatrixNV {
    float    matrix[3][4];
} VkTransformMatrixNV;

typedef struct VkAccelerationStructureInstanceNV {
    VkTransformMatrixNV           transform;
    uint32_t                      instanceCustomIndex:24;
    uint32_t                      mask:8;
    uint32_t                      instanceShaderBindingTableRecordOffset:24;
    VkFlags                       flags:8;
    uint64_t                      accelerationStructureReference;
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
        void setTransform(const TransformMatrix& transform) noexcept { this->transform = transform; }
        void setInstanceIndex(uint32_t instanceIndex) noexcept { instanceCustomIndex = instanceIndex; }
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
        void forceGeometryNoOpaque(bool noOpaque) noexcept { if (noOpaque) flags |= VK_GEOMETRY_INSTANCE_FORCE_NO_OPAQUE_BIT_NV;
                                                             else flags &= ~VK_GEOMETRY_INSTANCE_FORCE_NO_OPAQUE_BIT_NV; }
        bool geometryIsOpaque() const noexcept { return flags & VK_GEOMETRY_INSTANCE_FORCE_OPAQUE_BIT_NV; }
        bool geometryIsNoOpaque() const noexcept { return flags & VK_GEOMETRY_INSTANCE_FORCE_NO_OPAQUE_BIT_NV; }
        void setAccelerationStructure(std::shared_ptr<const AccelerationStructure> accelerationStructure);
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
            const Sharing& sharing = Sharing());
        uint32_t getInstanceCount() const noexcept { return (uint32_t)instances.size(); }
        AccelerationStructureInstance& getInstance(uint32_t instanceIndex) noexcept { return instances[instanceIndex]; }
        const AccelerationStructureInstance& getInstance(uint32_t instanceIndex) const noexcept { return instances[instanceIndex]; }
        bool update(std::shared_ptr<CommandBuffer> cmdBuffer,
            uint32_t firstInstance, 
            uint32_t instanceCount);

    private:
        std::shared_ptr<SrcTransferBuffer> stagingBuffer;
        std::vector<AccelerationStructureInstance> instances;
    };
#endif // VK_NV_ray_tracing
} // namespace magma
