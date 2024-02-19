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

namespace magma
{
    class Buffer;
    class CommandBuffer;
    class RenderPass;
    class DescriptorPool;
    class DescriptorSet;
    class PipelineCache;
    class GraphicsPipeline;
    class Allocator;

    namespace aux
    {
        /* Shader-based 8x12 glyph renderer.
           Simple, but sub-optimal solution to draw text on the screen.
           Supports only ASCII printable characters (32 - 127).
           See: https://www.shadertoy.com/view/Mt2GWD. */

        class TextShader : public Base
        {
        public:
            explicit TextShader(std::shared_ptr<RenderPass> renderPass,
                uint32_t maxChars = 1024,
                std::shared_ptr<Allocator> allocator = nullptr,
                std::shared_ptr<PipelineCache> pipelineCache = nullptr);
            void draw(std::shared_ptr<CommandBuffer> cmdBuffer) const noexcept;
            void begin();
            void end();
            void print(uint32_t x, uint32_t y,
                uint32_t color,
                const char *format, ...);

        private:
            struct Uniforms;
            struct String;
            struct Glyph;
            struct DescriptorSetTable;
            const uint32_t maxChars;
            uint32_t numChars = 0;
            std::shared_ptr<Allocator> allocator;
            std::shared_ptr<Buffer> uniforms;
            std::shared_ptr<Buffer> stringBuffer;
            std::shared_ptr<Buffer> glyphBuffer;
            std::shared_ptr<DescriptorPool> descriptorPool;
            std::shared_ptr<DescriptorSetLayout> descriptorSetLayout;
            std::shared_ptr<DescriptorSet> descriptorSet;
            std::shared_ptr<GraphicsPipeline> pipeline;
            std::unique_ptr<DescriptorSetTable> setTable;
            uint32_t width = 0;
            uint32_t height = 0;
            std::list<String> strings;
            Glyph *chars = nullptr;
            uint32_t offset = 0;
        };
    } // namespace aux
} // namespace magma
