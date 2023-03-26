/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2023 Victor Coda.

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
    namespace descriptor
    {
        /* Base template class of descriptor array. */

        template<class Type, uint32_t Size>
        class DescriptorArray : public Descriptor
        {
        public:
            constexpr uint32_t getArraySize() const noexcept { return Size; }

        protected:
            DescriptorArray(VkDescriptorType descriptorType, uint32_t binding) noexcept:
                Descriptor(descriptorType, Size, binding) {}

            std::array<Type, Size> descriptors = {};
        };
    } // namespace descriptor
} // namespace magma
