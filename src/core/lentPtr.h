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
    /* Holds a reference to native or shared pointer.
       Don't intend to manage the lifetime of underlying
       object, instead has non-destructible and non-
       copyable semantics. */

    template<class T>
    class lent_ptr
    {
    public:
        lent_ptr(T *native) noexcept:
            ptr(native)
        {}

        template<class T2>
        lent_ptr(const std::shared_ptr<T2>& shared) noexcept:
            ptr(shared.get()),
            ref(shared)
        {}

        template<class T2>
        lent_ptr(const std::unique_ptr<T2>& unique) noexcept:
            ptr(unique.get())
        {}

        template<class T2>
        lent_ptr(const std::weak_ptr<T2>& weak) noexcept:
            ptr(nullptr),
            ref(weak)
        {}

        lent_ptr(const lent_ptr&) = delete;
        lent_ptr& operator=(const lent_ptr&) = delete;

        template<class T2>
        lent_ptr(lent_ptr<T2>&& other) noexcept:
            ptr(other.ptr),
            ref(std::move(other.ref))
        {
            other.ptr = nullptr;
        }

        template<class T2>
        lent_ptr& operator=(lent_ptr<T2>&& other) noexcept
        {
            if (this != &other)
            {
                ptr = other.ptr;
                ref = std::move(other.ref);
                other.ptr = nullptr;
            }
            return *this;
        }

        bool shared() const noexcept { return !ref.expired(); }
        T *get() const noexcept { return ptr; }
        std::shared_ptr<T> get_shared() const noexcept { return ref.lock(); }

        void reset() noexcept
        {
            ptr = nullptr;
            ref.reset();
        }

        T *operator->() const noexcept
        {
            MAGMA_ASSERT(ptr);
            return ptr;
        }

        T& operator*() const noexcept
        {
            MAGMA_ASSERT(ptr);
            return *ptr;
        }

        operator bool() const noexcept { return ptr != nullptr; }

    private:
        T *ptr;
        std::weak_ptr<T> ref;

        template<class T2>
        friend class lent_ptr;
    };
} // namespace magma
