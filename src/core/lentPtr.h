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
    /* Holds a reference to native or shared pointer.
       It is not intended to manage the lifetime of the
       underlying object, but instead has non-destructive
       and non-copyable semantics. */

    template<class T>
    class lent_ptr final
    {
    public:
        lent_ptr(T *p) noexcept;
        lent_ptr(std::nullptr_t) noexcept;
        template<class T2> lent_ptr(const std::unique_ptr<T2>& p) noexcept;
        template<class T2> lent_ptr(const std::shared_ptr<T2>& p) noexcept;
        template<class T2> lent_ptr(const std::weak_ptr<T2>& ref) noexcept;
        template<class T2> lent_ptr(const variant_ptr<T2>& p) noexcept;
        lent_ptr(const lent_ptr&) = delete;
        template<class T2> lent_ptr(lent_ptr<T2>&& other) noexcept;

        T *get() const noexcept { return pointer; }
        std::shared_ptr<T> get_shared() const noexcept { return ref.lock(); }
        bool shareable() const noexcept;
        void reset() noexcept;

        lent_ptr& operator=(const lent_ptr&) = delete;
        template<class T2> lent_ptr& operator=(lent_ptr<T2>&& other) noexcept;

        T *operator->() const noexcept;
        T& operator*() const noexcept;
        explicit operator bool() const noexcept;

        bool operator<(const lent_ptr<T>& p) noexcept;
        bool operator>(const lent_ptr<T>& p) noexcept;
        bool operator<=(const lent_ptr<T>& p) noexcept;
        bool operator>=(const lent_ptr<T>& p) noexcept;
        bool operator==(const lent_ptr<T>& p) noexcept;
        bool operator!=(const lent_ptr<T>& p) noexcept;

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

        bool operator<(const variant_ptr<T>& p) noexcept;
        bool operator>(const variant_ptr<T>& p) noexcept;
        bool operator<=(const variant_ptr<T>& p) noexcept;
        bool operator>=(const variant_ptr<T>& p) noexcept;
        bool operator==(const variant_ptr<T>& p) noexcept;
        bool operator!=(const variant_ptr<T>& p) noexcept;

    private:
        template<class T2> friend class lent_ptr;

        T *pointer;
        std::weak_ptr<T> ref;
    };
} // namespace magma

#include "lentPtr.inl"
