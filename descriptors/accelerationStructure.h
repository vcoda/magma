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
#include "descriptor.h"

namespace magma
{
#if defined(VK_KHR_acceleration_structure) || defined(VK_NV_ray_tracing)
    class AccelerationStructure;
#endif

    namespace descriptor
    {
        /* An acceleration structure is a descriptor type that is used to retrieve
           scene geometry from within shaders that are used for ray traversal.
           Shaders have read-only access to the memory. */

    #if defined(VK_KHR_acceleration_structure) || defined(VK_NV_ray_tracing)
        class AccelerationStructure : public Descriptor
        {
        public:
            AccelerationStructure(uint32_t binding) noexcept;
            void getWriteDescriptor(VkDescriptorSet dstSet,
                VkWriteDescriptorSet& writeDescriptorSet) const noexcept override;
            AccelerationStructure& operator=(std::shared_ptr<const magma::AccelerationStructure>) noexcept;

        private:
        #ifdef VK_KHR_acceleration_structure
            VkAccelerationStructureKHR handle;
            VkWriteDescriptorSetAccelerationStructureKHR writeDescriptorSetAccelerationStructure;
        #else
            VkAccelerationStructureNV handle;
            VkWriteDescriptorSetAccelerationStructureNV writeDescriptorSetAccelerationStructure;
        #endif
        };
    #endif // VK_KHR_acceleration_structure || VK_NV_ray_tracing
    } // namespace descriptor
} // namespace magma
