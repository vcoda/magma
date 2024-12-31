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
    /* Variant pointer that can hold unique or shared pointer. */

    template<class T>
    class variant_ptr
    {
    public:
        variant_ptr(std::nullptr_t) noexcept
        {}

        variant_ptr(std::unique_ptr<T> shared) noexcept:
            ptr(std::move(shared))
        {}

        variant_ptr(std::shared_ptr<T> unique) noexcept:
            ptr(std::move(unique))
        {}

        T *get() noexcept
        {
            try
            {
                auto visitor = [](auto& p) -> T* { return p.get(); };
                return std::visit(visitor, ptr);
            }
            catch (const std::bad_variant_access&)
            {
                return nullptr;
            }
        }

        const T *get() const noexcept
        {
            try
            {
                auto visitor = [](auto const& p) -> T* { return p.get(); };
                return std::visit(visitor, ptr);
            }
            catch (const std::bad_variant_access&)
            {
                return nullptr;
            }
        }

        T *operator->() noexcept
        {
            return get();
        }

        const T *operator->() const noexcept
        {
            return get();
        }

        T& operator*() noexcept
        {
            return *get();
        }

        const T& operator*() const noexcept
        {
            return *get();
        }

        operator bool() const noexcept { return get() != nullptr; }

    private:
        std::variant<std::unique_ptr<T>, std::shared_ptr<T>> ptr;
    };
} // namespace magma
