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
#include <memory>
#include "../api/vulkan.h"
#include "../nonCopyable.h"

namespace magma
{
    class Device;
    class Framebuffer;
    class ColorAttachment2D;
    class DepthStencilAttachment2D;
    class SwapchainColorAttachment2D;
    class ImageView;
    class RenderPass;
    class IAllocator;

    namespace aux
    {
        /* Auxiliary framebuffer class that is responsible for management of
           color, depth/stencil and resolve attachments, image view reinterpretation,
           render pass and framebuffer construction. */

        class Framebuffer : public sys::NonCopyable
        {
        public:
            virtual std::shared_ptr<ImageView> getColorView() const noexcept { return colorView; }
            std::shared_ptr<RenderPass> getRenderPass() const noexcept { return renderPass; }
            std::shared_ptr<magma::Framebuffer> getFramebuffer() const noexcept { return framebuffer; }
            const VkExtent2D& getExtent() const noexcept { return extent; }
            virtual uint32_t getSampleCount() const noexcept = 0;

        protected:
            Framebuffer(const VkExtent2D& extent):
                extent(extent) {}
            Framebuffer(const VkExtent3D& extent)
            {
                this->extent.width = extent.width;
                this->extent.height = extent.height;
            }

        protected:
            std::shared_ptr<ImageView> colorView;
            std::shared_ptr<ImageView> depthStencilView;
            std::shared_ptr<RenderPass> renderPass;
            std::shared_ptr<magma::Framebuffer> framebuffer;
            VkExtent2D extent;
        };

        class NonMultisampleFramebuffer : public Framebuffer
        {
        public:
            NonMultisampleFramebuffer(std::shared_ptr<Device> device,
                const VkFormat colorFormat,
                const VkFormat depthStencilFormat,
                const VkExtent2D& extent,
                const VkComponentMapping& swizzle = {
                    VK_COMPONENT_SWIZZLE_IDENTITY,
                    VK_COMPONENT_SWIZZLE_IDENTITY,
                    VK_COMPONENT_SWIZZLE_IDENTITY,
                    VK_COMPONENT_SWIZZLE_IDENTITY},
                std::shared_ptr<IAllocator> allocator = nullptr);
            virtual uint32_t getSampleCount() const noexcept override { return 1; }

        private:
            std::shared_ptr<ColorAttachment2D> color;
            std::shared_ptr<DepthStencilAttachment2D> depthStencil;
        };

        class MultisampleFramebuffer : public Framebuffer
        {
        public:
            MultisampleFramebuffer(std::shared_ptr<Device> device,
                const VkFormat colorFormat,
                const VkFormat depthStencilFormat,
                const VkExtent2D& extent,
                uint32_t sampleCount,
                const VkComponentMapping& swizzle = {
                    VK_COMPONENT_SWIZZLE_IDENTITY,
                    VK_COMPONENT_SWIZZLE_IDENTITY,
                    VK_COMPONENT_SWIZZLE_IDENTITY,
                    VK_COMPONENT_SWIZZLE_IDENTITY},
                std::shared_ptr<IAllocator> allocator = nullptr);
            virtual std::shared_ptr<ImageView> getColorView() const noexcept override { return resolveView; }
            virtual uint32_t getSampleCount() const noexcept override;

        private:
            std::shared_ptr<ColorAttachment2D> color;
            std::shared_ptr<DepthStencilAttachment2D> depthStencil;
            std::shared_ptr<ColorAttachment2D> resolve;
            std::shared_ptr<ImageView> resolveView;
        };

        class SwapchainFramebuffer : public Framebuffer
        {
        public:
            SwapchainFramebuffer(std::shared_ptr<SwapchainColorAttachment2D> color,
                VkFormat depthFormat = VK_FORMAT_UNDEFINED,
                const VkComponentMapping& swizzle = {
                    VK_COMPONENT_SWIZZLE_IDENTITY,
                    VK_COMPONENT_SWIZZLE_IDENTITY,
                    VK_COMPONENT_SWIZZLE_IDENTITY,
                    VK_COMPONENT_SWIZZLE_IDENTITY},
                std::shared_ptr<IAllocator> allocator = nullptr);
            virtual uint32_t getSampleCount() const noexcept override { return 1; }

        private:
            std::shared_ptr<DepthStencilAttachment2D> depthStencil;
        };
    } // namespace aux
} // namespace magma
