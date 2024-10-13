/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2024 Victor Coda.

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
    namespace core
    {
        template<class Type>
        class variant_ptr
        {
        public:
            variant_ptr(std::unique_ptr<Type> ptr) noexcept;
            variant_ptr(std::shared_ptr<Type> ptr) noexcept;
            Type *get();
            const Type *get() const;
            Type *operator->();
            const Type *operator->() const;
            Type& operator*();
            const Type& operator*() const;
            operator bool() const;

        private:
            std::variant<std::unique_ptr<Type>, std::shared_ptr<Type>> ptr;
        };
    } // namespace core
} // namespace magma

#include "variantPtr.inl"
