/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2020 Victor Coda.

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
#include "exception.h"

namespace magma
{
    namespace exception
    {
        /* Vulkan instance doesn't support requested extension. */

        class UnsupportedInstanceExtension : public Exception
        {
        public:
            explicit UnsupportedInstanceExtension(const char *extension,
                const source_location& location) noexcept:
                Exception(extension, location) {}
        };

        /* Logical device doesn't support requested extension. */

        class UnsupportedDeviceExtension : public Exception
        {
        public:
            explicit UnsupportedDeviceExtension(const char *extension,
                const source_location& location) noexcept:
                Exception(extension, location) {}
        };
    } // namespace exception
} // namespace magma
