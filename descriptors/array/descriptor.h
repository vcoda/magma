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
        namespace array
        {
            /* Base class of array descriptor.
               Holds reference to an array element of type <DescriptorType>
               that is located inside ***DescriptorArray instance. */

            template<class Type>
            class Descriptor
            {
            protected:
                Descriptor(Type& descriptor, VkFlags requiredUsage, bool& updated) noexcept;

                Type& descriptor;
                const VkFlags requiredUsage;
                bool& updated;
            };

            /* Base class of image descriptor. */

            class BaseImageDescriptor : public Descriptor<VkDescriptorImageInfo>
            {
            protected:
                BaseImageDescriptor(VkDescriptorImageInfo& descriptor, VkImageType& imageType,
                    VkFlags requiredUsage, bool& updated) noexcept;
                void setImageType(std::shared_ptr<const ImageView> imageView) noexcept;

            private:
                VkImageType& imageType;
            };
        } // namespace array
    } // namespace descriptor
} // namespace magma

#include "descriptor.inl"

