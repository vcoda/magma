/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2026 Victor Coda.

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
    typedef uint32_t BindlessHandle;

    namespace descriptor
    {
        /* Thread-safe pool of variable-sized descriptors like
           images, buffers, samplers etc. */

        template<class Descriptor>
        class DescriptorPool
        {
        public:
            DescriptorPool();
            void reserve(std::size_t capacity);
            BindlessHandle insert(const Descriptor& descriptor);
            bool erase(BindlessHandle handle);
            void clear() noexcept;
            const Descriptor *data() const noexcept { return descriptors.data(); }
            std::size_t size() const noexcept;
            std::size_t numDescriptors() const noexcept;

        private:
            std::vector<Descriptor> descriptors;
            std::vector<bool> assigned;
            std::vector<BindlessHandle> unusedHandles;
            std::shared_ptr<core::Spinlock> lock;
        };
    } // namespace descriptor
} // namespace magma

#include "descriptorPool.inl"
