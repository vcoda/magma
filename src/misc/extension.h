/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2026 Victor Coda.

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
#include "../exceptions/errorResult.h"

namespace magma
{
    /* Holds a pointer to the Vulkan ICD proc address. */

    template<class Fn, bool Instance>
    class ProcAddress
    {
    public:
        operator Fn() const noexcept { return addr; }
        operator bool() const noexcept { return addr != nullptr; }
        bool instance() const noexcept { return Instance; }

    protected:
        ProcAddress(PFN_vkVoidFunction addr) noexcept:
            addr(reinterpret_cast<Fn>(addr)) {}
        void checkAddress(const char *extensionName) const;

    private:
        const Fn addr;
    };

    /* Represents enabled instance extension dispatchable command. */

    template<class Fn>
    class InstanceExtension final : public ProcAddress<Fn, true>
    {
    public:
        explicit InstanceExtension(VkInstance instance,
            const char *procName) noexcept;
        explicit InstanceExtension(VkInstance instance,
            const char *procName,
            const char *extensionName);
    };

    /* In order to support systems with multiple Vulkan
       implementations, the function pointers returned by
       vkGetInstanceProcAddr may point to dispatch code
       that calls a different real implementation for
       different VkDevice objects or their child objects.
       The overhead of the internal dispatch for VkDevice
       objects can be avoided by obtaining device-specific
       function pointers for any commands that use a device
       or device-child object as their dispatchable object. */

    template<class Fn>
    class DeviceExtension final : public ProcAddress<Fn, false>
    {
    public:
        explicit DeviceExtension(VkDevice device,
            const char *procName) noexcept;
        explicit DeviceExtension(VkDevice device,
            const char *procName,
            const char *extensionName);
    };
} // namespace magma

#include "extension.inl"

/* Users of these macros should have implemented
   getNativeInstance() or getNativeDevice() methods
   in the scope of extension function call. */

#define MAGMA_INSTANCE_EXTENSION(proc) static const magma::InstanceExtension<PFN_##proc> proc(getNativeInstance(), MAGMA_STRINGIZE(proc))
#define MAGMA_DEVICE_EXTENSION(proc) static const magma::DeviceExtension<PFN_##proc> proc(getNativeDevice(), MAGMA_STRINGIZE(proc))

#define MAGMA_REQUIRED_INSTANCE_EXTENSION(proc, extensionName) static const magma::InstanceExtension<PFN_##proc> proc(getNativeInstance(), MAGMA_STRINGIZE(proc), extensionName)
#define MAGMA_REQUIRED_DEVICE_EXTENSION(proc, extensionName) static const magma::DeviceExtension<PFN_##proc> proc(getNativeDevice(), MAGMA_STRINGIZE(proc), extensionName)
