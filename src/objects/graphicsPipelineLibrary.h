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
#include "pipelineLibrary.h"

namespace magma
{
    class PipelineLayout;
    class PipelineShaderStage;
    class RenderPass;

    class VertexInputState;
    struct InputAssemblyState;
    struct TesselationState;
    struct RasterizationState;
    struct MultisampleState;
    struct DepthStencilState;
    struct ColorBlendState;
    class ViewportState;

    /* Allows the separate compilation of four distinct parts
       of graphics pipelines, with the intent of allowing
       faster pipeline loading for applications reusing the
       same shaders or state in multiple pipelines. Each part
       can be independently compiled into a graphics pipeline
       library, with a final link step required to create an
       executable pipeline that can be bound to a command buffer. */

#ifdef VK_EXT_graphics_pipeline_library
    class GraphicsPipelineLibrary : public PipelineLibrary
    {
    public:
        explicit GraphicsPipelineLibrary(std::shared_ptr<Device> device,
            std::shared_ptr<IAllocator> allocator = nullptr) noexcept;
        void compileVertexInputInterface(const VertexInputState& vertexInputState,
            const InputAssemblyState& inputAssemblyState,
            VkPipelineCreateFlags flags = 0);
        void compilePreRasterizationShaders(const std::vector<PipelineShaderStage>& preRasterizationShaderStages,
            const TesselationState& tesselationState,
            const ViewportState& viewportState,
            const RasterizationState& rasterizationState,
            const std::vector<VkDynamicState>& dynamicStates,
            std::unique_ptr<PipelineLayout> layout,
            VkPipelineCreateFlags flags = 0);
        void compileFragmentShader(const PipelineShaderStage& shaderStage,
            const MultisampleState& multisampleState,
            const DepthStencilState& depthStencilState,
            std::unique_ptr<PipelineLayout> layout,
            std::shared_ptr<RenderPass> renderPass,
            uint32_t subpass,
            VkPipelineCreateFlags flags = 0);
        void compileFragmentOutputInterface(const MultisampleState& multisampleState,
            const ColorBlendState& colorBlendState,
            std::shared_ptr<RenderPass> renderPass,
            uint32_t subpass,
            VkPipelineCreateFlags flags = 0);
    };
#endif // VK_EXT_graphics_pipeline_library
} // namespace magma
