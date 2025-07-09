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
#include "accelerationStructure.h"

namespace magma
{
    /* Each instance in the top-level acceleration structure
       contains a reference to a bottom-level acceleration
       structure as well as an instance transform plus information
       required to index into the shader bindings. The top-level
       acceleration structure is what is bound to the acceleration
       descriptor, for example to trace inside the shader in the
       ray tracing pipeline. */

#ifdef VK_KHR_acceleration_structure
    class TopLevelAccelerationStructure : public AccelerationStructure
    {
    public:
        explicit TopLevelAccelerationStructure(std::shared_ptr<Device> device,
            const AccelerationStructureInstances& instances,
            VkAccelerationStructureBuildTypeKHR buildType,
            VkBuildAccelerationStructureFlagsKHR buildFlags,
            std::shared_ptr<Allocator> allocator = nullptr,
            VkAccelerationStructureCreateFlagsKHR flags = 0,
            const Sharing& sharing = Sharing(),
            const StructureChain& extendedInfo = StructureChain());
        explicit TopLevelAccelerationStructure(std::shared_ptr<Device> device,
            VkDeviceSize deserializedSize,
            VkAccelerationStructureBuildTypeKHR buildType,
            VkBuildAccelerationStructureFlagsKHR buildFlags,
            std::shared_ptr<Allocator> allocator = nullptr,
            VkAccelerationStructureCreateFlagsKHR flags = 0,
            const Sharing& sharing = Sharing(),
            const StructureChain& extendedInfo = StructureChain());
        void build(const AccelerationStructureInstances& instances,
            void *scratchBuffer,
            lent_ptr<DeferredOperation> deferredOperation = nullptr);
        void update(const AccelerationStructureInstances& instances,
            void *scratchBuffer,
            lent_ptr<DeferredOperation> deferredOperation = nullptr);

    private:
        VkResult rebuild(VkBuildAccelerationStructureModeKHR mode,
            const AccelerationStructureInstances& instances,
            void *scratchBuffer,
            lent_ptr<DeferredOperation> deferredOperation);
    };
#endif // VK_KHR_acceleration_structure
} // namespace magma
