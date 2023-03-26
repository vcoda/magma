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
#include "pch.h"
#pragma hdrstop
#include "descriptor.h"
#include "../objects/buffer.h"
#include "../objects/bufferView.h"
#include "../objects/image.h"
#include "../objects/imageView.h"

namespace magma
{
namespace descriptor
{
Descriptor::Descriptor(VkDescriptorType descriptorType, uint32_t descriptorCount, uint32_t binding_) noexcept
{
    binding.binding = binding_;
    binding.descriptorType = descriptorType;
    binding.descriptorCount = descriptorCount;
    binding.stageFlags = 0;
    binding.pImmutableSamplers = nullptr;
}

DescriptorArray::DescriptorArray(VkDescriptorType descriptorType, uint32_t descriptorCount, uint32_t binding) noexcept:
    Descriptor(descriptorType, descriptorCount, binding)
{}
void Descriptor::setImageType(VkImageType imageType_) noexcept
{
    if (imageType != VK_IMAGE_TYPE_MAX_ENUM)
    {   // Descriptor should have the same image type
        MAGMA_ASSERT(imageType_ == imageType);
    }
    imageType = imageType_;
}
} // namespace descriptor
} // namespace magma
