/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2020 Victor Coda.

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
#include "../core/noncopyable.h"
#include "../descriptors/binding.h"
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
        /* Shader-based 8x12 glyph renderer. See https://www.shadertoy.com/view/Mt2GWD.
           Simple, but sub-optimal solution to draw text on the screen.
           Supports only ASCII printable characters (32 - 127). */

        class TextShader : public core::NonCopyable
        {
        public:
            explicit TextShader(const uint32_t maxChars, const uint32_t maxStrings,
                std::shared_ptr<RenderPass> renderPass,
                std::shared_ptr<PipelineCache> pipelineCache = nullptr,
                std::shared_ptr<Allocator> allocator = nullptr);
            void draw(std::shared_ptr<CommandBuffer> cmdBuffer) const noexcept;
            void begin();
            void end();
            void print(uint32_t x,
                uint32_t y,
                uint32_t color,
                const char *format,
                ...);

        private:
            struct Uniforms;
            struct String;
            struct alignas(16) Glyph
            {
                float c[4];
            };

            struct SetLayout : DescriptorSetDeclaration
            {
                binding::UniformBuffer uniforms = 0;
                binding::StorageBuffer stringBuffer = 1;
                binding::StorageBuffer glyphBuffer = 2;
                MAGMA_REFLECT(&uniforms, &stringBuffer, &glyphBuffer)
            };

            const uint32_t maxChars, maxStrings;
            std::shared_ptr<Buffer> uniforms;
            std::shared_ptr<Buffer> stringBuffer;
            std::shared_ptr<Buffer> glyphBuffer;
            std::shared_ptr<DescriptorPool> descriptorPool;
            std::shared_ptr<DescriptorSetLayout> descriptorSetLayout;
            std::shared_ptr<DescriptorSet> descriptorSet;
            std::shared_ptr<GraphicsPipeline> pipeline;
            SetLayout setLayout;
            uint32_t width = 0;
            uint32_t height = 0;
            std::vector<Glyph> glyphs;
            std::list<String> strings;
            std::vector<Glyph> chars;
            uint32_t offset = 0;
        };
    } // namespace aux
} // namespace magma
