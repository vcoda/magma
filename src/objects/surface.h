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
#include "model/nondispatchable.h"

namespace magma
{
    class Instance;

    /* A surface object abstracts a native platform surface or
       window object for use with Vulkan. Separate platform-specific
       extensions each provide a function for creating a surface
       object for the respective platform. */

#ifdef VK_KHR_surface
    class Surface : public NonDispatchable<VkSurfaceKHR>
    {
    public:
        ~Surface();
        const std::shared_ptr<Instance>& getInstance() const noexcept { return instance; }

    protected:
        Surface(std::shared_ptr<Instance> instance,
            std::shared_ptr<IAllocator> allocator);

        std::shared_ptr<Instance> instance;
    };
#endif // VK_KHR_surface

    /* Display surface describes the configuration of a single
       plane within a complete display configuration. */

#ifdef VK_KHR_display
    class DisplayMode;

    class DisplaySurface : public Surface
    {
    public:
        explicit DisplaySurface(std::shared_ptr<Instance> instance,
            lent_ptr<const DisplayMode> displayMode,
            uint32_t planeIndex,
            uint32_t planeStackIndex,
            VkSurfaceTransformFlagBitsKHR transform,
            VkDisplayPlaneAlphaFlagBitsKHR alphaMode,
            std::shared_ptr<IAllocator> allocator = nullptr);
        uint32_t getPlaneIndex() const noexcept { return planeIndex; }
        uint32_t getPlaneStackIndex() const noexcept { return planeStackIndex; }
        VkSurfaceTransformFlagBitsKHR getTransform() const noexcept { return transform; }
        VkDisplayPlaneAlphaFlagBitsKHR getAlphaMode() const noexcept { return alphaMode; }

    private:
        const uint32_t planeIndex;
        const uint32_t planeStackIndex;
        const VkSurfaceTransformFlagBitsKHR transform;
        const VkDisplayPlaneAlphaFlagBitsKHR alphaMode;
    };
#endif // VK_KHR_display

    /* Provides a mechanism to create surface objects
       independently of any window system or display device.
       The presentation operation for a swapchain created
       from a headless surface is by default a no-op,
       resulting in no externally-visible result. */

#ifdef VK_EXT_headless_surface
    class HeadlessSurface : public Surface
    {
    public:
        explicit HeadlessSurface(std::shared_ptr<Instance> instance,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkHeadlessSurfaceCreateFlagsEXT flags = 0);
    };
#endif // VK_EXT_headless_surface
} // namespace magma
