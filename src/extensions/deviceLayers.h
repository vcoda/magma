/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2025 Victor Coda.

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
#include "extensionRegistry.h"

namespace magma
{
    class PhysicalDevice;

    /* Keep list of layers in alphabetical order! */

    class DeviceLayers : public ExtensionRegistry<VkLayerProperties>
    {
    public:
        // Advanced Micro Devices, Inc.
        const VkBool32 AMD_switchable_graphics: 1;

        // Khronos Group
        const VkBool32 KHRONOS_validation: 1;

        explicit DeviceLayers(const std::shared_ptr<PhysicalDevice>&);
    };

    MAGMA_TYPEDEF_MANAGED_PTR(DeviceLayers)
} // namespace magma
