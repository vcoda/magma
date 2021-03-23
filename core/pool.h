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
#include <unordered_set>
#include "noncopyable.h"

namespace magma
{
    namespace core
    {
        /* Set of existing objects. */

		template<typename Type>
        class Pool final : public NonCopyable
        {
        public:
            void add(const Type *obj) noexcept
			{
				std::lock_guard<std::mutex> guard(mtx);
				try {
					pool.insert(obj);
				} catch (...) {}
			}

            void remove(const Type *obj) noexcept
			{
				std::lock_guard<std::mutex> guard(mtx);
				try {
					auto it = pool.find(obj);
					MAGMA_ASSERT(it != pool.end());
					if (it != pool.end())
						pool.erase(it);
				} catch (...) {}
			}

            uint32_t count() const noexcept
			{
				std::lock_guard<std::mutex> guard(mtx);
				try {
					return static_cast<uint32_t>(pool.size());
				} catch (...) {
					return 0;
				}
			}

            template<typename DerivedType> 
			void forEach(const std::function<void(const DerivedType *obj)>& cb) const noexcept
			{
				std::lock_guard<std::mutex> guard(mtx);
				try {
					std::for_each(pool.begin(), pool.end(),
						[&cb](const Type *base)
						{
							const DerivedType *derived = dynamic_cast<const DerivedType *>(base);
							if (derived)
								cb(derived);
						});
				} catch (...) {}
			}

		private:
			std::unordered_set<const Type *> pool;
            mutable std::mutex mtx;
        };
    } // namespace core
} // namespace magma
