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
#include <list>
#include <unordered_set>
#include "../objects/shaderModule.h"
#include "../states/rasterizationState.h"
#include "../states/multiSampleState.h"
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

    namespace helpers
    {
        /* Sometimes I miss immediate mode from OpenGL 1.x era.
           This class serves as a replacement for it when you don't want
           to mess around with vertex buffer mapping, data copy, state changes etc. */
        class ImmediateRender
        {
            struct Vertex
            {
                float x, y, z, w;
                float nx, ny, nz, psize;
                float r, g, b, a;
                float u, v;
            };

            struct Transform
            {
                float m[16] = {1.f, 0.f, 0.f, 0.f,
                               0.f, 1.f, 0.f, 0.f,
                               0.f, 0.f, 1.f, 0.f,
                               0.f, 0.f, 0.f, 1.f};
            };

            struct Primitive
            {
                std::shared_ptr<GraphicsPipeline> pipeline;
                Transform transform;
                uint32_t vertexCount;
                uint32_t firstVertex;
            };

        public:
            ImmediateRender(uint32_t maxVertexCount,
                std::shared_ptr<Device> device,
                std::shared_ptr<PipelineCache> cache,
                std::shared_ptr<PipelineLayout> layout,
                std::shared_ptr<RenderPass> renderPass,
                std::shared_ptr<IAllocator> allocator = nullptr);
            void setVertexShader(const VertexShaderStage& vertexShader);
            void setFragmentShader(const FragmentShaderStage& fragmentShader);
            void setRasterizationState(const RasterizationState& state);
            void setMultisampleState(const MultisampleState& state);
            void setDepthStencilState(const DepthStencilState& state);
            void setColorBlendState(const ColorBlendState& state);
            void setIdentity();
            void setTransform(const float transform[16]);
            bool beginPrimitive(VkPrimitiveTopology topology);
            bool endPrimitive();
            void normal(float x, float y, float z);
            void normal(const float n[3]);
            void color(float r, float g, float b, float a = 1.f);
            void color(const float c[4]);
            void color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
            void color(const uint8_t c[4]);
            void texCoord(float u, float v);
            void texCoord(const float tc[2]);
            void pointSize(float size);
            void vertex(float x, float y, float z = 0.f, float w = 1.f);
            void vertex(const float v[4]);
            template<typename Vertex2> void vertex2(const Vertex2& v);
            template<typename Vertex3> void vertex3(const Vertex3& v);
            template<typename Vertex4> void vertex4(const Vertex4& v);
            bool commitPrimitives(std::shared_ptr<CommandBuffer>& cmdBuffer,
                bool clear = true);
            bool reset();
            uint32_t getVertexCount() const { return vertexCount; }
            uint32_t getPrimitiveCount() const { return MAGMA_COUNT(primitives); }

        private:
            std::shared_ptr<ShaderModule> createVertexShader();
            std::shared_ptr<ShaderModule> createFragmentShader();
            std::shared_ptr<GraphicsPipeline> createPipelineState(VkPrimitiveTopology topology);

        private:
            const uint32_t maxVertexCount;
            std::shared_ptr<Device> device;
            std::shared_ptr<PipelineCache> cache;
            std::shared_ptr<PipelineLayout> layout;
            std::shared_ptr<RenderPass> renderPass;
            std::shared_ptr<VertexBuffer> vertexBuffer;
            VertexShaderStage vertexShader;
            FragmentShaderStage fragmentShader;
            RasterizationState rasterizationState;
            MultisampleState multisampleState;
            DepthStencilState depthStencilState;
            ColorBlendState colorBlendState;
            Transform transform;
            std::list<Primitive> primitives;
            std::unordered_set<std::shared_ptr<GraphicsPipeline>> pipelines;
            uint32_t vertexCount = 0;
            Vertex *vert = nullptr;
            Vertex curr = {};
            bool insidePrimitive = false;
        };
    } // namespace helpers
} // namespace magma

#include "immediateRender.inl"
