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
#include "loadStoreOp.h"

namespace magma
{
    /* An attachment description describes the properties of an attachment
       including its format, sample count, and how its contents are treated
       at the beginning and end of each render pass instance. */

    struct AttachmentDescription : VkAttachmentDescription
    {
        constexpr AttachmentDescription(VkFormat format,
            uint32_t sampleCount,
            const LoadStoreOp& colorDepthOp,
            const LoadStoreOp& stencilOp,
            VkImageLayout initialLayout,
            VkImageLayout finalLayout) noexcept;
        constexpr AttachmentDescription(const LoadStoreOp& colorDepthOp,
            const LoadStoreOp& stencilOp,
            VkImageLayout initialLayout,
            VkImageLayout finalLayout) noexcept;
        constexpr hash_t hash() const noexcept;
    };
} // namespace magma

#include "attachment.inl"
