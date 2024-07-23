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
    /* The defined header structure for the serialized data consists of:
        * VK_UUID_SIZE bytes of data matching VkPhysicalDeviceIDProperties::driverUUID.
        * VK_UUID_SIZE bytes of data identifying the compatibility for comparison
          using vkGetDeviceAccelerationStructureCompatibilityKHR.
        * A 64-bit integer of the total size matching the value queried using
          VK_QUERY_TYPE_ACCELERATION_STRUCTURE_SERIALIZATION_SIZE_KHR.
        * A 64-bit integer of the deserialized size to be passed in to
          VkAccelerationStructureCreateInfoKHR::size.
        * A 64-bit integer of the count of the number of acceleration structure
          handles following. This value matches the value queried using
          VK_QUERY_TYPE_ACCELERATION_STRUCTURE_SERIALIZATION_BOTTOM_LEVEL_POINTERS_KHR.
          This will be zero for a bottom-level acceleration structure.
          For top-level acceleration structures this number is implementation-
          dependent; the number of and ordering of the handles may not
          match the instance descriptions which were used to build the
          acceleration structure. */

#ifdef VK_KHR_acceleration_structure
    struct AccelerationStructureHeader
    {
        uint8_t driverUUID[VK_UUID_SIZE] = {};
        uint8_t compatibilityUUID[VK_UUID_SIZE] = {};
        uint64_t serializedSize = 0ull; // Including header
        uint64_t deserializedSize = 0ull;
        uint64_t bottomLevelAccelerationStructurePointerCount = 0ull;
        VkDeviceAddress bottomLevelAccelerationStructurePointers[1] = {MAGMA_NULL}; // Array of D3D12_GPU_VIRTUAL_ADDRESS in Direct3D 12
    };
#endif // VK_KHR_acceleration_structure
} // namespace magma
