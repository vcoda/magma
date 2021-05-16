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

namespace magma
{
    class Device;
    class ShaderModule;
    class IAllocator;

    struct RasterizationState;

    namespace aux
    {
        /* Vertex shader that rasterizes axis-aligned screen-space bounding box.
           https://www.khronos.org/registry/OpenGL/extensions/NV/NV_fill_rectangle.txt
           https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VK_NV_fill_rectangle.html */

        class FillRectangleVertexShader : public core::NonCopyable
        {
        public:
            explicit FillRectangleVertexShader(std::shared_ptr<Device> device,
                std::shared_ptr<IAllocator> allocator = nullptr);
            std::shared_ptr<ShaderModule> getShader() const noexcept { return shader; }
            const char *getEntryPointName() const noexcept;
            const RasterizationState& getRasterizationState() const noexcept;

        private:
            const bool hasFillRectangle;
            std::shared_ptr<ShaderModule> shader;
        };
    } // namespace aux
} // namespace magma
