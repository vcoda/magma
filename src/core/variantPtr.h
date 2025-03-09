/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2025 Victor Coda.

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
    /* Variant pointer that can hold either unique or shared pointer. */

    template<class T>
    class variant_ptr final
    {
    public:
        variant_ptr(std::nullptr_t) noexcept;
        variant_ptr(std::unique_ptr<T> p) noexcept;
        variant_ptr(std::shared_ptr<T> p) noexcept;
        variant_ptr(variant_ptr<T>&& other) noexcept;

        T *get() const noexcept;

        variant_ptr& operator=(std::unique_ptr<T>&& p) noexcept;
        variant_ptr& operator=(std::shared_ptr<T>&& p) noexcept;
        variant_ptr& operator=(variant_ptr<T>&& other) noexcept;

        T *operator->() const noexcept;
        T& operator*() const noexcept;
        explicit operator bool() const noexcept;

        bool operator<(const variant_ptr<T>& p) noexcept;
        bool operator>(const variant_ptr<T>& p) noexcept;
        bool operator<=(const variant_ptr<T>& p) noexcept;
        bool operator>=(const variant_ptr<T>& p) noexcept;
        bool operator==(const variant_ptr<T>& p) noexcept;
        bool operator!=(const variant_ptr<T>& p) noexcept;

        bool operator<(const T *p) noexcept;
        bool operator>(const T *p) noexcept;
        bool operator<=(const T *p) noexcept;
        bool operator>=(const T *p) noexcept;
        bool operator==(const T *p) noexcept;
        bool operator!=(const T *p) noexcept;

        bool operator<(const std::unique_ptr<T>& p) noexcept;
        bool operator>(const std::unique_ptr<T>& p) noexcept;
        bool operator<=(const std::unique_ptr<T>& p) noexcept;
        bool operator>=(const std::unique_ptr<T>& p) noexcept;
        bool operator==(const std::unique_ptr<T>& p) noexcept;
        bool operator!=(const std::unique_ptr<T>& p) noexcept;

        bool operator<(const std::shared_ptr<T>& p) noexcept;
        bool operator>(const std::shared_ptr<T>& p) noexcept;
        bool operator<=(const std::shared_ptr<T>& p) noexcept;
        bool operator>=(const std::shared_ptr<T>& p) noexcept;
        bool operator==(const std::shared_ptr<T>& p) noexcept;
        bool operator!=(const std::shared_ptr<T>& p) noexcept;

    private:
        std::variant<std::unique_ptr<T>, std::shared_ptr<T>> var;
    };
} // namespace magma

#include "variantPtr.inl"
