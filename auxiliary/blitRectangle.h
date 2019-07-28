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
#include "../misc/clearValue.h"

namespace magma
{
    class Device;
    class Framebuffer;
    class ImageView;
    class RenderPass;
    class ShaderModule;
    class PipelineShaderStage;
    class Sampler;
    class DescriptorPool;
    class DescriptorSet;
    class DescriptorSetLayout;
    class PipelineLayout;
    class GraphicsPipeline;
    class CommandBuffer;
    class IAllocator;

    namespace aux
    {
        /* Auxiliary object that helps quickly blit source texture
           into destination framebuffer. */

        class BlitRectangle : public internal::NonCopyable
        {
        public:
            explicit BlitRectangle(std::shared_ptr<RenderPass> renderPass,
                std::shared_ptr<IAllocator> allocator = nullptr);
            explicit BlitRectangle(std::shared_ptr<RenderPass> renderPass,
                const PipelineShaderStage& vertexShader,
                const PipelineShaderStage& fragmentShader,
                std::shared_ptr<IAllocator> allocator = nullptr);
            void blit(const std::shared_ptr<Framebuffer>& bltDst,
                const std::shared_ptr<ImageView>& bltSrc,
                const std::shared_ptr<CommandBuffer>& cmdBuffer,
                VkFilter filter,
                bool negativeViewportHeight = false,
                const char *labelName  = nullptr,
                uint32_t labelColor = 0xFFFFFFFF) const noexcept;

        private:
            std::shared_ptr<ShaderModule> createShader(std::shared_ptr<Device>,
                std::shared_ptr<IAllocator>, bool) const;

        private:
            std::shared_ptr<RenderPass> renderPass;
            std::shared_ptr<DescriptorPool> descriptorPool;
            std::shared_ptr<DescriptorSet> descriptorSet;
            std::shared_ptr<DescriptorSetLayout> descriptorSetLayout;
            std::shared_ptr<Sampler> nearestSampler;
            std::shared_ptr<Sampler> bilinearSampler;
            std::shared_ptr<Sampler> cubicSampler;
            std::shared_ptr<PipelineLayout> pipelineLayout;
            std::shared_ptr<GraphicsPipeline> pipeline;
            std::vector<ClearValue> clearValues;
            mutable std::shared_ptr<ImageView> prevBltSrc;
            mutable VkFilter prevFilter = VK_FILTER_NEAREST;
        };
    } // namespace aux
} // namespace magma
