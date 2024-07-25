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
    /* Structure specifying a 3x4 affine transformation matrix. */

#ifdef VK_KHR_acceleration_structure
    struct TransformMatrix : VkTransformMatrixKHR
    {
        constexpr TransformMatrix() noexcept;
        template<int M, int N>
        constexpr TransformMatrix(const float (&mat)[M][N]) noexcept;
        template<class Matrix>
        TransformMatrix(const Matrix& mat) noexcept;
    };

    /* Acceleration structure instances can be built into top-level
       acceleration structures. Each acceleration structure instance
       is a separate entry in the top-level acceleration structure
       which includes all the geometry of a bottom-level acceleration
       structure at a transformed location. Multiple instances
       can point to the same bottom level acceleration structure. */

    struct AccelerationStructureInstance : VkAccelerationStructureInstanceKHR
    {
        constexpr AccelerationStructureInstance() noexcept;
    };
#endif // VK_KHR_acceleration_structure

    /* Structure specifying a transform in SRT decomposition.
       The effective derived transform is given by T * R * S, where

           [sx a  b  pvx]
       S = [0  sy c  pvy]
           [0  0  sz pvz]

           [1 0 0 tx]
       T = [0 1 0 ty]
           [0 0 1 tz]

       R = [qx, qy, qz, qw], where

       [qx, qy, qz] = sin(theta/2) * [ax, ay, az] */

#ifdef VK_NV_ray_tracing_motion_blur
    struct SRTData : VkSRTDataNV
    {
        constexpr SRTData() noexcept;
    };

    /* Acceleration structure motion instances can be built into
       top-level acceleration structures. Each acceleration structure
       instance includes a type of motion and parameters to determine
       the motion of the instance over time. */

    struct AccelerationStructureMatrixMotionInstance : VkAccelerationStructureMatrixMotionInstanceNV
    {
        constexpr AccelerationStructureMatrixMotionInstance() noexcept;
    };

    struct AccelerationStructureSRTMotionInstance : VkAccelerationStructureSRTMotionInstanceNV
    {
        constexpr AccelerationStructureSRTMotionInstance() noexcept;
    };

    struct AccelerationStructureMotionInstance : VkAccelerationStructureMotionInstanceNV
    {
        constexpr AccelerationStructureMotionInstance() noexcept;

        const uint64_t padding = 0ull;
    };
#endif // VK_NV_ray_tracing_motion_blur
} // namespace magma

#include "accelerationStructureInstance.inl"
