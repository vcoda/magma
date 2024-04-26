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
#include "pch.h"
#pragma hdrstop
#include "application.h"

namespace magma
{
Application::Application(const char *applicationName, uint32_t applicationVersion,
    const char *engineName, uint32_t engineVersion, uint32_t apiVersion,
    const StructureChain& extendedInfo /* default */) noexcept:
    VkApplicationInfo{
        VK_STRUCTURE_TYPE_APPLICATION_INFO,
        extendedInfo.firstNode(),
        applicationName,
        applicationVersion,
        engineName,
        engineVersion,
        apiVersion
    }
{
    MAGMA_ASSERT(applicationName);
    MAGMA_ASSERT(strlen(applicationName) > 0);
    MAGMA_ASSERT(engineName);
    MAGMA_ASSERT(strlen(engineName) > 0);
}
} // namespace magma
