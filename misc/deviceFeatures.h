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
#include "../core/noncopyable.h"

namespace magma
{
    class Device;

    /*  Helper class that allows to determine whether
        some extended device features have been enabled or not.
        Incapsulates boilerplate code that gets to the point. */

    class DeviceFeatures : core::NonCopyable
    {
        DeviceFeatures(std::shared_ptr<const Device> device) noexcept;
        friend Device;

    public:
        struct FormatFeaturesSupport
        {
            VkBool32 linear: 1;
            VkBool32 optimal: 1;
            VkBool32 buffer: 1;
        };

        FormatFeaturesSupport checkFormatFeaturesSupport(VkFormat format,
            VkFormatFeatureFlags flags) const noexcept;
        bool maintenanceEnabled(uint8_t index) const noexcept;
        bool negativeViewportHeightEnabled() const noexcept;
        bool separateDepthStencilLayoutsEnabled() const noexcept;
        bool extendedLinesEnabled() const noexcept;
        bool stippledLinesEnabled() const noexcept;

    private:
        std::weak_ptr<const Device> parent;
    };
} // namespace magma