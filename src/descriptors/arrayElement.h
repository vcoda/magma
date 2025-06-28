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
#include "descriptorSetLayoutBinding.h"

namespace magma::descriptor
{
    /* Base element of array of buffer or image descriptors.
       Allows to mark array of descriptors as dirty if
       single element of this array has been updated. */

    template<class Type>
    class ArrayElement
    {
    protected:
        ArrayElement(DescriptorSetLayoutBinding *binding, Type& element, VkFlags usage) noexcept:
            element(element), usage(usage), binding(binding) {}
        void setImageType(VkImageType imageType) noexcept { binding->setImageType(imageType); }
        void setDirty() noexcept { binding->dirty = true; }

        Type& element;
        const VkFlags usage;

    private:
        DescriptorSetLayoutBinding *binding;
    };
} // namespace magma::descriptor
