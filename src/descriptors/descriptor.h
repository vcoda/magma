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
#include "binding.h"

namespace magma
{
    namespace descriptor
    {
        /* Descriptor is coupled with VkDescriptorSetLayoutBinding
           structure. While layout binding stores binding index and
           properties of binded descriptor, descriptor itself manages
           descriptor structure of type <Info> to store properties of
           underlying objects like buffers, images, samplers,
           acceleration structures, uniform blocks etc. */

        template<class Info>
        class Descriptor : public Binding
        {
        protected:
            Descriptor(VkDescriptorType descriptorType, uint32_t binding) noexcept:
                Binding(descriptorType, 1, binding) {}

            Info descriptor = {};
        };

        /* Base template class of descriptor array. */

        template<class Info, uint32_t Size>
        class DescriptorArray : public Binding
        {
        public:
            constexpr uint32_t getArraySize() const noexcept { return Size; }

        protected:
            DescriptorArray(VkDescriptorType descriptorType, uint32_t binding) noexcept:
                Binding(descriptorType, Size, binding) {}

            std::array<Info, Size> descriptors = {};
        };
    } // namespace descriptor
} // namespace magma
