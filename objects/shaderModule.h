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
#include <vector>
#include "handle.h"

namespace magma
{
    /* Shader modules contain shader code and one or more entry points.
       Shaders are selected from a shader module by specifying an entry point.
       The stages of a pipeline can use shaders that come from different modules.
       The shader code defining a shader module must be in the SPIR-V format. */

    class ShaderModule : public NonDispatchable<VkShaderModule>
    {
    public:
        explicit ShaderModule(std::shared_ptr<Device> device,
            const uint32_t *bytecode,
            size_t bytecodeSize,
            std::shared_ptr<IAllocator> allocator = nullptr);
        explicit ShaderModule(std::shared_ptr<Device> device,
            const std::vector<uint32_t>& bytecode,
            std::shared_ptr<IAllocator> allocator = nullptr);
        ~ShaderModule();
    };
} // namespace magma
