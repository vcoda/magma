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
    /* Each individual descriptor binding is specified by a
        descriptor type, a count (array size) of the number
        of descriptors in the binding, a set of shader stages
        that can access the binding, and (if using immutable
        samplers) an array of sampler descriptors. */

    class DescriptorSetLayoutBinding : public VkDescriptorSetLayoutBinding
    {
    public:
        VkImageType getImageType() const noexcept { return imageType; }
        bool modified() const noexcept { return dirty; }
        virtual bool associatedWithResource() const noexcept = 0;
        virtual void write(VkDescriptorSet dstSet,
            VkWriteDescriptorSet& writeDescriptorSet) const noexcept = 0;

    protected:
        DescriptorSetLayoutBinding(VkDescriptorType descriptorType,
            uint32_t descriptorCount,
            uint32_t binding) noexcept;
        void setImageType(VkImageType imageType) noexcept;

        VkImageType imageType;
        mutable bool dirty;
    };
} // namespace magma

#include "descriptorSetLayoutBinding.inl"
