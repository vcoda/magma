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

namespace magma
{
    class Buffer;
    class CommandBuffer;
    class RenderPass;
    class DescriptorPool;
    class DescriptorSet;
    class DescriptorSetLayout;
    class PipelineCache;
    class PipelineLayout;
    class GraphicsPipeline;
    class Allocator;

    namespace aux
    {
        /* Shader-based 8x12 glyph renderer.
           Simple, but sub-optimal solution to draw text on the screen.
           Supports only ASCII printable characters (32 - 127).
           See: https://www.shadertoy.com/view/Mt2GWD. */

        class TextShader : public IClass
        {
        public:
            explicit TextShader(std::shared_ptr<const RenderPass> renderPass,
                uint32_t maxChars = 1024,
                std::shared_ptr<Allocator> allocator = nullptr,
                const std::unique_ptr<PipelineCache>& pipelineCache = nullptr);
            void draw(lent_ptr<CommandBuffer> cmdBuffer) const noexcept;
            void begin();
            void end();
            bool print(uint32_t x, uint32_t y,
                uint32_t color,
                const char *format, ...);

        private:
            struct String;
            struct Glyph;
            struct DescriptorSetTable;
            struct PushConstants;
            const uint32_t maxStrings;
            const uint32_t maxChars;
            std::shared_ptr<Allocator> allocator;
            std::unique_ptr<Buffer> stringBuffer;
            std::unique_ptr<Buffer> charBuffer;
            std::shared_ptr<DescriptorPool> descriptorPool;
            std::unique_ptr<DescriptorSetLayout> descriptorSetLayout;
            std::unique_ptr<DescriptorSet> descriptorSet;
            std::unique_ptr<GraphicsPipeline> pipeline;
            std::unique_ptr<DescriptorSetTable> setTable;
            String *strings = nullptr;
            uint32_t stringCount = 0;
            Glyph *chars = nullptr;
            uint32_t charCount = 0;
            uint32_t offset = 0;
        };
    } // namespace aux
} // namespace magma
