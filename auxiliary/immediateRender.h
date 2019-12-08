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
#include "../internal/noncopyable.h"
#include "../shaders/shaderStages.h"
#include "../states/vertexLayouts.h"
#include "../states/rasterizationState.h"
#include "../states/multisampleState.h"
#include "../states/depthStencilState.h"
#include "../states/colorBlendState.h"

namespace magma
{
    class Device;
    class CommandBuffer;
    class GraphicsPipeline;
    class PipelineLayout;
    class PipelineCache;
    class VertexBuffer;
    class RenderPass;
    class IAllocator;

    class VertexInputState;
    struct InputAssemblyState;

    namespace aux
    {
        class GraphicsPipelineCache;

        /* Sometimes I miss immediate mode from OpenGL 1.x era.
           This class serves as a replacement for it when you don't want
           to mess around with vertex buffer mapping, data copy,
           state changes etc. */

        class ImmediateRender : public internal::NonCopyable
        {
        public:
            explicit ImmediateRender(uint32_t maxVertexCount,
                std::shared_ptr<Device> device,
                std::shared_ptr<PipelineCache> cache,
                std::shared_ptr<PipelineLayout> layout,
                std::shared_ptr<RenderPass> renderPass,
                std::shared_ptr<IAllocator> allocator = nullptr);
            void setVertexShader(const VertexShaderStage& vertexShader) noexcept;
            void setFragmentShader(const FragmentShaderStage& fragmentShader) noexcept;
            void setRasterizationState(const RasterizationState& state) noexcept;
            void setMultisampleState(const MultisampleState& state) noexcept;
            void setDepthStencilState(const DepthStencilState& state) noexcept;
            void setColorBlendState(const ColorBlendState& state) noexcept;
            void setLineWidth(float width) noexcept;
            void setIdentity() noexcept;
            void setTransform(const float transform[16]) noexcept;
            uint32_t getVertexCount() const noexcept;
            uint32_t getPrimitiveCount() const noexcept;
            bool beginPrimitive(VkPrimitiveTopology topology, const char *labelName = nullptr, uint32_t labelColor = 0xFFFFFFFF);
            bool endPrimitive(bool loop = false) noexcept;
            bool commitPrimitives(std::shared_ptr<CommandBuffer>& cmdBuffer, bool clear = true) noexcept;
            bool reset() noexcept;
            // Per-vertex attributes
            void normal(float x, float y, float z) noexcept;
            void normal(const float n[3]) noexcept;
            void color(float r, float g, float b, float a = 1.f) noexcept;
            void color(const float c[4]) noexcept;
            void color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = std::numeric_limits<uint8_t>::max()) noexcept;
            void color(const uint8_t c[4]) noexcept;
            void texcoord(float u, float v) noexcept;
            void texcoord(const float uv[2]) noexcept;
            void pointSize(float size) noexcept;
            void vertex(float x, float y, float z = 0.f, float w = 1.f) noexcept;
            void vertex(const float v[4]) noexcept;

        private:
            std::shared_ptr<ShaderModule> createShader(bool vertexShader) const;
            std::shared_ptr<GraphicsPipeline> lookupPipeline(VkPrimitiveTopology);

        private:
            struct Vertex
            {
                vectors::float4 position;
                vectors::float4 normalPSize;
                vectors::float4 color;
                vectors::float2 texcoord;
            };

            struct Transform
            {
                float m[16] = {
                    1.f, 0.f, 0.f, 0.f,
                    0.f, 1.f, 0.f, 0.f,
                    0.f, 0.f, 1.f, 0.f,
                    0.f, 0.f, 0.f, 1.f};
            };

            struct Primitive
            {
                std::shared_ptr<GraphicsPipeline> pipeline;
                float lineWidth;
                Transform transform;
                uint32_t vertexCount;
                uint32_t firstVertex;
                const char *labelName;
                uint32_t labelColor;
            };

            const uint32_t maxVertexCount;
            std::shared_ptr<Device> device;
            std::shared_ptr<PipelineLayout> layout;
            std::shared_ptr<RenderPass> renderPass;
            std::shared_ptr<IAllocator> allocator;
            std::shared_ptr<VertexBuffer> vertexBuffer;
            std::shared_ptr<GraphicsPipelineCache> pipelineCache;
            std::list<Primitive> primitives;
            VertexShaderStage vertexShader;
            FragmentShaderStage fragmentShader;
            RasterizationState rasterizationState;
            MultisampleState multisampleState;
            DepthStencilState depthStencilState;
            ManagedColorBlendState colorBlendState;
            float lineWidth = 1.f;
            Transform transform;
            uint32_t vertexCount = 0;
            Vertex *pvertex = nullptr;
            Vertex current = {};
            bool insidePrimitive = false;
        };
    } // namespace aux
} // namespace magma

#include "immediateRender.inl"
