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
#include "../core/destructible.h"

namespace magma
{
    class ShaderReflection;

    namespace reflection
    {
        /* An abstract interface to query shader reflection for validatation purposes.
           By design, library doesn't perform any filesystem I/O operations. Instead, user have
           to create an implementation of this interface to provide his loading logic. */

        class IShaderReflectionFactory : public core::IDestructible
        {
        public:
            virtual std::shared_ptr<const ShaderReflection> getReflection(const std::string& shaderFileName) = 0;
        };
    } // namespace reflection
} // namespace magma
