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
#include "../shaders/shaderStages.h"
#include "../states/vertexFormat.h"
#include "../states/rasterizationState.h"
#include "../states/multisampleState.h"
#include "../states/depthStencilState.h"
#include "../states/multiColorBlendState.h"

namespace magma
{
    class Device;
    class CommandBuffer;
    class GraphicsPipeline;
    class PipelineLayout;
    class PipelineCache;
    class DynamicVertexBuffer;
    class RenderPass;
    class Allocator;

    struct InputAssemblyState;

    namespace aux
    {
        class GraphicsPipelineCache;

        /* Sometimes I miss immediate mode from OpenGL 1.x era.
           This class serves as a replacement for it when you don't
           want to mess around with vertex buffer mapping, data copy,
           render state changes etc. */

        class ImmediateRender : public IClass
        {
        public:
            explicit ImmediateRender(const uint32_t maxVertexCount,
                std::shared_ptr<RenderPass> renderPass,
                std::shared_ptr<PipelineLayout> layout,
                std::shared_ptr<Allocator> allocator = nullptr);
            const std::shared_ptr<Device>& getDevice() const noexcept { return device; }
            const std::unique_ptr<PipelineCache>& getPipelineCache() const noexcept;
            uint32_t getMaxVertexCount() const noexcept { return maxVertexCount; }
            uint32_t getVertexCount() const noexcept { return vertexCount; }
            uint32_t getPrimitiveCount() const noexcept { return core::countof(primitives); }
            // Render states setup
            void setVertexShader(const VertexShaderStage& vertexShader) noexcept;
            void setFragmentShader(const FragmentShaderStage& fragmentShader) noexcept;
            void setRasterizationState(const RasterizationState& state) noexcept;
            void setMultisampleState(const MultisampleState& state) noexcept;
            void setDepthStencilState(const DepthStencilState& state) noexcept;
            void setColorBlendState(const ColorBlendState& state) noexcept;
            void setLineWidth(float width) noexcept;
            void setLineStippleFactor(uint32_t stippleFactor) noexcept;
            void setLineStipplePattern(uint16_t stipplePattern) noexcept;
            void setIdentity() noexcept;
            void setTransform(const float matrix[4][4]) noexcept;
            void setViewProjTransform(const float matrix[4][4]) noexcept;
            // Primitive generation
            bool beginPrimitive(VkPrimitiveTopology topology,
                const char *labelName = nullptr,
                uint32_t labelColor = 0xFFFFFFFF);
            bool endPrimitive(bool loop = false) noexcept;
            bool commitPrimitives(lent_ptr<CommandBuffer> cmdBuffer,
                bool freePrimitiveList = true) noexcept;
            bool reset() noexcept;
            // Per-vertex attributes
            void normal(float x, float y, float z) noexcept;
            void normal(const float n[3]) noexcept;
            void color(float r, float g, float b, float a = 1.f) noexcept;
            void color(const float c[4]) noexcept;
            void color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = std::numeric_limits<uint8_t>::max()) noexcept;
            void color(const uint8_t c[4]) noexcept;
            void texCoord(float u, float v) noexcept;
            void texCoord(const float uv[2]) noexcept;
            void pointSize(float size) noexcept;
            void vertex(float x, float y, float z = 0.f, float w = 1.f) noexcept;
            void vertex(const float v[4]) noexcept;

        private:
            struct Vertex;
            struct Primitive;
            struct PushConstants;
            std::shared_ptr<GraphicsPipeline> lookupPipeline(VkPrimitiveTopology,
                bool wideLineState, bool stippledLineState);

            const uint32_t maxVertexCount;
            const bool wideLinesEnabled;
            const bool stippledLinesEnabled;
            std::shared_ptr<Device> device;
            std::shared_ptr<RenderPass> renderPass;
            std::shared_ptr<PipelineLayout> sharedLayout;
            std::unique_ptr<GraphicsPipelineCache> pipelineCache;
            std::unique_ptr<DynamicVertexBuffer> vertexBuffer;
            std::list<Primitive> primitives;
            std::vector<PipelineShaderStage> shaderStages;
            RasterizationState rasterizationState;
            MultisampleState multisampleState;
            DepthStencilState depthStencilState;
            MultiColorBlendState colorBlendState;
            VkBool32 stippledLineState;
            float lineWidth = 1.f;
            uint32_t lineStippleFactor = 1;
            uint16_t lineStipplePattern = 0b1111111111111111;
            float world[4][4];
            float viewProj[4][4];
            Vertex *current = nullptr;
            bool insidePrimitive = false;
            uint32_t vertexCount = 0;
        };

        struct ImmediateRender::Vertex
        {
            Float4 position;
            Float4 normalPSize;
            Float4 color;
            Float2 texCoord;
        };

        struct ImmediateRender::Primitive
        {
            VkBool32 wideLineState: 1;
            VkBool32 stippledLineState: 1;
            std::shared_ptr<GraphicsPipeline> pipeline;
            float lineWidth;
            uint32_t lineStippleFactor;
            uint16_t lineStipplePattern;
            float world[4][4];
            uint32_t vertexCount;
            uint32_t firstVertex;
            const char *labelName;
            uint32_t labelColor;
        };

        struct ImmediateRender::PushConstants
        {
            float world[4][4];
            float viewProj[4][4];
        };
    } // namespace aux
} // namespace magma

#include "immediateRender.inl"
