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
    class Image;

#ifdef VK_KHR_imageless_framebuffer
    class FramebufferAttachmentImage : public VkFramebufferAttachmentImageInfoKHR
    {
    public:
        FramebufferAttachmentImage() noexcept: VkFramebufferAttachmentImageInfoKHR{} {}
        explicit FramebufferAttachmentImage(VkImageUsageFlags usage,
            uint32_t width,
            uint32_t height,
            uint32_t layerCount,
            const std::vector<VkFormat>& viewFormats,
            VkImageCreateFlags flags = 0) noexcept;
        explicit FramebufferAttachmentImage(lent_ptr<const Image> image) noexcept;
        FramebufferAttachmentImage(const FramebufferAttachmentImage&) noexcept;
        FramebufferAttachmentImage(FramebufferAttachmentImage&&) noexcept;
        ~FramebufferAttachmentImage() { delete[] pViewFormats; }
        FramebufferAttachmentImage& operator=(const FramebufferAttachmentImage&) noexcept;
        VkExtent2D getExtent() const noexcept { return {width, height}; }
    };
#endif // VK_KHR_imageless_framebuffer
} // namespace magma
