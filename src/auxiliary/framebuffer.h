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

namespace magma
{
    class Device;
    class ImageView;
    class RenderPass;
    class Framebuffer;
    class SwapchainImage;
    class Allocator;

    struct MultisampleState;

    namespace aux
    {
        /* Auxiliary framebuffer that implements allocation of color,
           depth/stencil and resolve attachments, configures attachment
           descriptors and creates render pass and framebuffer. Can be
           created on top of front and back images of swapchain. */

        class Framebuffer
        {
        public:
            explicit Framebuffer(std::shared_ptr<Device> device,
                VkFormat colorFormat,
                VkFormat depthStencilFormat,
                const VkExtent2D& extent,
                uint32_t sampleCount,
                bool loadOpClear,
                bool depthStencilSampled,
                bool explicitResolve = false,
                std::shared_ptr<Allocator> allocator = nullptr,
                const VkComponentMapping& swizzling = {
                    VK_COMPONENT_SWIZZLE_IDENTITY,
                    VK_COMPONENT_SWIZZLE_IDENTITY,
                    VK_COMPONENT_SWIZZLE_IDENTITY,
                    VK_COMPONENT_SWIZZLE_IDENTITY},
                VkRenderPassCreateFlags renderPassFlags = 0,
                VkFramebufferCreateFlags framebufferFlags = 0);
            explicit Framebuffer(std::shared_ptr<Device> device,
                const std::initializer_list<VkFormat>& colorFormats,
                VkFormat depthStencilFormat,
                const VkExtent2D& extent,
                bool loadOpClear,
                bool depthStencilSampled,
                std::shared_ptr<Allocator> allocator = nullptr,
                const std::vector<VkComponentMapping>& swizzles = {},
                VkRenderPassCreateFlags renderPassFlags = 0,
                VkFramebufferCreateFlags framebufferFlags = 0);
            explicit Framebuffer(std::shared_ptr<SwapchainImage> colorAttachment,
                std::shared_ptr<ImageView> sharedDepthStencilView = nullptr,
                std::shared_ptr<Allocator> allocator = nullptr,
                const VkComponentMapping& swizzling = {
                    VK_COMPONENT_SWIZZLE_IDENTITY,
                    VK_COMPONENT_SWIZZLE_IDENTITY,
                    VK_COMPONENT_SWIZZLE_IDENTITY,
                    VK_COMPONENT_SWIZZLE_IDENTITY},
                VkRenderPassCreateFlags renderPassFlags = 0,
                VkFramebufferCreateFlags framebufferFlags = 0);
            virtual ~Framebuffer();
            const std::vector<VkFormat>& getColorFormats() const noexcept { return colorFormats; }
            VkFormat getDepthStencilFormat() const noexcept { return depthStencilFormat; }
            const VkExtent2D& getExtent() const noexcept;
            uint32_t getSampleCount() const noexcept { return sampleCount; }
            const MultisampleState& getMultisampleState() const noexcept;
            bool requiresColorClear() const noexcept { return loadOpClear; }
            bool resolvedExplicitly() const noexcept { return explicitResolve; }
            const std::shared_ptr<ImageView>& getColorView() const noexcept { return colorViews.front(); }
            const std::shared_ptr<ImageView>& getResolveView() const noexcept { return resolveView; }
            const std::shared_ptr<ImageView>& getDepthStencilView() const noexcept { return depthStencilView; }
            const std::vector<std::shared_ptr<ImageView>>& getColorViews() const noexcept { return colorViews; }
            const std::shared_ptr<const RenderPass>& getRenderPass() const noexcept;
            const std::unique_ptr<magma::Framebuffer>& getFramebuffer() const noexcept { return framebuffer; }

        private:
            VkImageLayout optimalDepthStencilLayout(std::shared_ptr<Device> device,
               VkFormat depthStencilFormat,
               bool sampled) const;

            const std::vector<VkFormat> colorFormats;
            const VkFormat depthStencilFormat;
            const uint32_t sampleCount;
            const bool loadOpClear;
            const bool explicitResolve;
            std::vector<std::shared_ptr<ImageView>> colorViews;
            std::shared_ptr<ImageView> resolveView;
            std::shared_ptr<ImageView> depthStencilView;
            std::unique_ptr<magma::Framebuffer> framebuffer;
        };
    } // namespace aux
} // namespace magma
