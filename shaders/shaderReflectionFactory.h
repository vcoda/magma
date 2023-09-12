/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2022 Victor Coda.

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
    class ShaderReflection;

    /* An abstract interface to query shader reflection for
       validatation purposes. By design, library doesn't performs
       any filesystem I/O operations. Instead, user have to provide
       an implementation of his loading logic. */

    class IShaderReflectionFactory : public IDestructible
    {
    public:
        virtual std::shared_ptr<const ShaderReflection> getReflection(const std::string& shaderFileName) = 0;
    };

    MAGMA_TYPEDEF_SHARED_PTR_INTERFACE(ShaderReflectionFactory);
} // namespace magma
