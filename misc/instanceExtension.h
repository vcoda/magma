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
#pragma once
#include "extension.h"
#include "../objects/instance.h"

namespace magma
{
    /* Represents enabled instance extension dispatchable command. */

    template<class ExtProc>
    class InstanceExtension final : public Extension<ExtProc, false>
    {
    public:
        explicit InstanceExtension(VkInstance instance,
            const char *name) noexcept:
            Extension<ExtProc, false>(vkGetInstanceProcAddr(instance, name)) {}
        explicit InstanceExtension(VkInstance instance,
            const char *name,
            const char *extensionName):
            InstanceExtension(instance, name)
        {
            Extension<ExtProc, false>::verify(extensionName);
        }
    };
} // namespace magma

#define MAGMA_INSTANCE_EXTENSION(proc) static InstanceExtension<PFN_##proc> proc(MAGMA_HANDLE(instance), MAGMA_STRINGIZE(proc))
#define MAGMA_REQUIRED_INSTANCE_EXTENSION(proc, extensionName) static InstanceExtension<PFN_##proc> proc(MAGMA_HANDLE(instance), MAGMA_STRINGIZE(proc), extensionName)
