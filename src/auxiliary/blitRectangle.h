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
#include "base.h"
#include "../objects/descriptorSetLayout.h"
#include "../misc/clearValue.h"

namespace magma
{
    class Device;
    class Framebuffer;
    class ImageView;
    class RenderPass;
    class ShaderModule;
    class Specialization;
    class Sampler;
    class DescriptorPool;
    class DescriptorSet;
    class GraphicsPipeline;
    class PipelineCache;
    class CommandBuffer;
    class IAllocator;

    namespace aux
    {
        /* Auxiliary object that helps to quickly blit
           source texture into destination framebuffer. */

        class BlitRectangle : public Base
        {
        public:
            explicit BlitRectangle(std::shared_ptr<RenderPass> renderPass,
                std::shared_ptr<PipelineCache> pipelineCache = nullptr,
                std::shared_ptr<IAllocator> allocator = nullptr);
            explicit BlitRectangle(std::shared_ptr<RenderPass> renderPass,
                std::shared_ptr<ShaderModule> fragmentShader,
                std::shared_ptr<Specialization> specialization = nullptr,
                std::shared_ptr<PipelineCache> pipelineCache = nullptr,
                std::shared_ptr<IAllocator> allocator = nullptr);
            void blit(std::shared_ptr<CommandBuffer> cmdBuffer,
                std::shared_ptr<const ImageView> imageView,
                VkFilter filter,
                const VkRect2D& rc,
                bool negativeViewportHeight = false) const noexcept;

        private:
            struct DescriptorSetTable;
            std::shared_ptr<RenderPass> renderPass;
            std::shared_ptr<DescriptorPool> descriptorPool;
            std::shared_ptr<Sampler> nearestSampler;
            std::shared_ptr<Sampler> bilinearSampler;
            std::shared_ptr<Sampler> cubicSampler;
            std::shared_ptr<GraphicsPipeline> pipeline;
            std::vector<ClearValue> clearValues;
            mutable std::list<DescriptorSetTable> setTables;
            mutable std::map<std::shared_ptr<const ImageView>, std::shared_ptr<DescriptorSet>> descriptorSets;
        };
    } // namespace aux
} // namespace magma
