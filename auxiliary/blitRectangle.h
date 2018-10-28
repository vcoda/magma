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
#include "../nonCopyable.h"

namespace magma
{
    class Device;
    class Framebuffer;
    class ImageView;
    class RenderPass;
    class VertexShaderStage;
    class FragmentShaderStage;
    class Sampler;
    class DescriptorPool;
    class DescriptorSet;
    class DescriptorSetLayout;
    class PipelineLayout;
    class GraphicsPipeline;
    class CommandPool;
    class CommandBuffer;
    class Semaphore;
    class Fence;
    class Queue;
    class IAllocator;

    namespace aux
    {
        class Framebuffer;

        class BlitRectangle : public NonCopyable
        {
        public:
            BlitRectangle(std::shared_ptr<aux::Framebuffer> framebuffer,
                std::shared_ptr<CommandPool> cmdPool,
                const VertexShaderStage& vertexShader,
                const FragmentShaderStage& fragmentShader);
            BlitRectangle(std::shared_ptr<RenderPass> renderPass,
                std::shared_ptr<magma::Framebuffer> framebuffer,
                std::shared_ptr<CommandPool> cmdPool,
                const VertexShaderStage& vertexShader,
                const FragmentShaderStage& fragmentShader);
            void setImageView(std::shared_ptr<ImageView> attachment,
                std::shared_ptr<Sampler> sampler = nullptr);
            void blit(std::shared_ptr<Semaphore> renderFinished,
                std::shared_ptr<Semaphore> blitFinished,
                std::shared_ptr<Fence> fence) const noexcept;

        private:
            std::shared_ptr<Device> device;
            std::shared_ptr<Queue> queue;
            std::shared_ptr<RenderPass> renderPass;
            std::shared_ptr<magma::Framebuffer> framebuffer;
            std::shared_ptr<DescriptorPool> descriptorPool;
            std::shared_ptr<DescriptorSetLayout> setLayout;
            std::shared_ptr<DescriptorSet> descriptorSet;
            std::shared_ptr<Sampler> nearestSampler;
            std::shared_ptr<GraphicsPipeline> pipeline;
            std::shared_ptr<PipelineLayout> pipelineLayout;
            std::shared_ptr<CommandPool> cmdPool;
            std::shared_ptr<CommandBuffer> cmdBuffer;
        };
    } // namespace aux
} // namespace magma
