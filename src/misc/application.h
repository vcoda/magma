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
#include "structureChain.h"

namespace magma
{
    /* IHVs (independent hardware vendors) can use information
       about application to provide application-specific
       optimizations for any program. */

    class Application final : public VkApplicationInfo
    {
    public:
        explicit Application(const char *applicationName,
            uint32_t applicationVersion,
            const char *engineName,
            uint32_t engineVersion,
            uint32_t apiVersion,
            const StructureChain& extendedInfo = StructureChain()) noexcept;
    };
} // namespace magma
