/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2020 Victor Coda.

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
#include "../core/noncopyable.h"
#include "../misc/format.h"

namespace magma
{
    class Device;
    class Framebuffer;
    class RenderPass;

    struct MultisampleState;

    namespace aux
    {
        /* Framebuffer's base class with shared functionality. */

        class Framebuffer
        {
        public:
            VkFormat getColorFormat() const noexcept { return colorFormat; }
            VkFormat getDepthStencilFormat() const noexcept { return depthStencilFormat; }
            bool hasColor() const noexcept { return colorFormat.valid(); }
            bool hasDepth() const noexcept;
            bool hasStencil() const noexcept;
            const VkExtent2D& getExtent() const noexcept;
            uint32_t getSampleCount() const noexcept { return sampleCount; }
            const MultisampleState& getMultisampleState() const noexcept;
            std::shared_ptr<RenderPass> getRenderPass() noexcept { return renderPass; }
            std::shared_ptr<const RenderPass> getRenderPass() const noexcept { return renderPass; }
            std::shared_ptr<magma::Framebuffer> getFramebuffer() noexcept { return framebuffer; }
            std::shared_ptr<const magma::Framebuffer> getFramebuffer() const noexcept { return framebuffer; }

        protected:
            explicit Framebuffer(VkFormat colorFormat,
                VkFormat depthStencilFormat,
                uint32_t sampleCount) noexcept;
            VkImageLayout finalDepthStencilLayout(std::shared_ptr<Device> device,
                const VkFormat depthStencilFormat, bool depthSampled) const;

            const Format colorFormat;
            const Format depthStencilFormat;
            const uint32_t sampleCount;
            std::shared_ptr<RenderPass> renderPass;
            std::shared_ptr<magma::Framebuffer> framebuffer;
        };
    } // namespace aux
} // namespace magma
