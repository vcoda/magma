/*
Magma - C++1x interface over Khronos Vulkan API.
Copyright (C) 2018 Victor Coda.

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
#include <vector>
#include "handle.h"

namespace magma
{
    class Device;
    class RenderPass;
    class ImageView;
   
    class Framebuffer : public NonDispatchable<VkFramebuffer>
    {
    public:
        Framebuffer(std::shared_ptr<const Device> device,
            std::shared_ptr<const RenderPass> renderPass,
            const std::vector<std::shared_ptr<const ImageView>>& attachments,
            VkFramebufferCreateFlags = 0);
        Framebuffer(std::shared_ptr<const Device> device,
            std::shared_ptr<const RenderPass> renderPass,
            std::shared_ptr<const ImageView> attachment,
            VkFramebufferCreateFlags = 0);
	    ~Framebuffer();
        const std::vector<std::shared_ptr<const ImageView>>& getAttachments() const { return attachments; }
        const VkExtent2D& getExtent() const { return extent; }

    private:
        std::vector<std::shared_ptr<const ImageView>> attachments;
        VkExtent2D extent;
    };
} // namespace magma
