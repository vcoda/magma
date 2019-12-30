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
#include "../detail/noncopyable.h"

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
        /* Abstract interface of Framebuffer object.
           Concrete implementations include MSAA and non-MSAA frame buffers
           as well as main window frame buffer. */

        class IFramebuffer : public detail::NonCopyable
        {
        public:
            virtual const VkExtent2D& getExtent() const noexcept = 0;
            virtual uint32_t getSampleCount() const noexcept = 0;
            virtual std::shared_ptr<ImageView> getColorView() noexcept = 0;
            virtual std::shared_ptr<const ImageView> getColorView() const noexcept = 0;
            virtual std::shared_ptr<RenderPass> getRenderPass() noexcept = 0;
            virtual std::shared_ptr<const RenderPass> getRenderPass() const noexcept = 0;
            virtual std::shared_ptr<Framebuffer> getFramebuffer() noexcept = 0;
            virtual std::shared_ptr<const Framebuffer> getFramebuffer() const noexcept = 0;
        };

        /* Framebuffer's base class with shared functionality. */

        class BaseFramebuffer : public IFramebuffer
        {
        public:
            virtual const VkExtent2D& getExtent() const noexcept override { return extent; }
            virtual uint32_t getSampleCount() const noexcept override { return 1; }
            virtual std::shared_ptr<RenderPass> getRenderPass() noexcept override { return renderPass; }
            virtual std::shared_ptr<const RenderPass> getRenderPass() const noexcept override { return renderPass; }
            virtual std::shared_ptr<ImageView> getColorView() noexcept override { return colorView; }
            virtual std::shared_ptr<const ImageView> getColorView() const noexcept override { return colorView; }
            virtual std::shared_ptr<Framebuffer> getFramebuffer() noexcept override { return framebuffer; }
            virtual std::shared_ptr<const Framebuffer> getFramebuffer() const noexcept override { return framebuffer; }

        protected:
            explicit BaseFramebuffer(const VkExtent2D& extent) noexcept;

        protected:
            VkExtent2D extent;
            std::shared_ptr<ImageView> colorView;
            std::shared_ptr<ImageView> depthStencilView;
            std::shared_ptr<RenderPass> renderPass;
            std::shared_ptr<Framebuffer> framebuffer;
        };

        /* Auxiliary frame buffer class that is responsible for management of
           color, depth/stencil and resolve attachments, image view reinterpretation,
           render pass and framebuffer construction. */

        class Framebuffer : public BaseFramebuffer
        {
        public:
            explicit Framebuffer(std::shared_ptr<Device> device,
                const VkFormat colorFormat,
                const VkFormat depthStencilFormat,
                const VkExtent2D& extent,
                const VkComponentMapping& swizzle = {
                    VK_COMPONENT_SWIZZLE_IDENTITY,
                    VK_COMPONENT_SWIZZLE_IDENTITY,
                    VK_COMPONENT_SWIZZLE_IDENTITY,
                    VK_COMPONENT_SWIZZLE_IDENTITY},
                std::shared_ptr<IAllocator> allocator = nullptr);

        private:
            std::shared_ptr<ColorAttachment2D> color;
            std::shared_ptr<DepthStencilAttachment2D> depthStencil;
        };

        /* Multi-sample frame buffer with resolve attachment. */

        class MultisampleFramebuffer : public BaseFramebuffer
        {
        public:
            explicit MultisampleFramebuffer(std::shared_ptr<Device> device,
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
            virtual uint32_t getSampleCount() const noexcept override;
            virtual std::shared_ptr<ImageView> getColorView() noexcept override { return resolveView; }
            virtual std::shared_ptr<const ImageView> getColorView() const noexcept override { return resolveView; }

        private:
            std::shared_ptr<ColorAttachment2D> color;
            std::shared_ptr<DepthStencilAttachment2D> depthStencil;
            std::shared_ptr<ColorAttachment2D> resolve;
            std::shared_ptr<ImageView> resolveView;
        };

        /* Main window frame buffer. */

        class SwapchainFramebuffer : public BaseFramebuffer
        {
        public:
            explicit SwapchainFramebuffer(std::shared_ptr<SwapchainColorAttachment2D> color,
                VkFormat depthFormat = VK_FORMAT_UNDEFINED,
                const VkComponentMapping& swizzle = {
                    VK_COMPONENT_SWIZZLE_IDENTITY,
                    VK_COMPONENT_SWIZZLE_IDENTITY,
                    VK_COMPONENT_SWIZZLE_IDENTITY,
                    VK_COMPONENT_SWIZZLE_IDENTITY},
                std::shared_ptr<IAllocator> allocator = nullptr);

        private:
            std::shared_ptr<DepthStencilAttachment2D> depthStencil;
        };
    } // namespace aux
} // namespace magma
