/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2021 Victor Coda.

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
        /* Thread safe unordered set with some sugar. */

        template<class Type>
        class ThreadSafeUnorderedSet : std::unordered_set<Type *>
        {
            typedef std::unordered_set<Type *> base;
            mutable std::mutex mtx;

        public:
            void insert(Type *obj)
            {
                std::lock_guard<std::mutex> guard(mtx);
                base::insert(obj);
            }

            void erase(Type *obj)
            {
                std::lock_guard<std::mutex> guard(mtx);
                auto it = base::find(obj);
                if (it != base::end())
                    base::erase(it);
            }

            uint32_t count() const noexcept
            {
                std::lock_guard<std::mutex> guard(mtx);
                return static_cast<uint32_t>(base::size());
            }

            template<class DerivedType>
            void forEach(const std::function<void(const DerivedType *obj)>& fn) const
            {
                std::lock_guard<std::mutex> guard(mtx);
                std::for_each(base::begin(), base::end(),
                    [&fn](const Type *it)
                    {
                        const DerivedType *obj = dynamic_cast<const DerivedType *>(it);
                        if (obj)
                            fn(obj);
                    });
            }
        };
    } // namespace core
} // namespace magma
