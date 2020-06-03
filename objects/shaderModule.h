/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2019 Victor Coda.

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
#include "nondispatchable.h"

namespace magma
{
    class Device;
    class ValidationCache;
    class ShaderReflection;
    class IAllocator;

    /* See https://www.khronos.org/registry/spir-v/specs/1.0/SPIRV.pdf
       2.3 Physical Layout of a SPIR-V Module and Instruction */
    typedef uint32_t SpirvWord;

    /* Shader modules contain shader code and one or more entry points.
       Shaders are selected from a shader module by specifying an entry point.
       The stages of a pipeline can use shaders that come from different modules.
       The shader code defining a shader module must be in the SPIR-V format. */

    class ShaderModule : public NonDispatchable<VkShaderModule>
    {
    public:
        explicit ShaderModule(std::shared_ptr<Device> device,
            const SpirvWord *bytecode,
            std::size_t bytecodeSize,
            std::size_t bytecodeHash = 0,
            VkShaderModuleCreateFlags flags = 0,
            bool reflect = false,
            std::shared_ptr<IAllocator> allocator = nullptr
#       ifdef VK_EXT_validation_cache
            ,std::shared_ptr<ValidationCache> validationCache = nullptr
#       endif
        );
        explicit ShaderModule(std::shared_ptr<Device> device,
            const std::vector<SpirvWord>& bytecode,
            std::size_t bytecodeHash = 0,
            VkShaderModuleCreateFlags flags = 0,
            bool reflect = false,
            std::shared_ptr<IAllocator> allocator = nullptr
#       ifdef VK_EXT_validation_cache
            ,std::shared_ptr<ValidationCache> validationCache = nullptr
#       endif
        );
        template<std::size_t WordCount>
        explicit ShaderModule(std::shared_ptr<Device> device,
            const SpirvWord (&bytecode)[WordCount],
            std::size_t bytecodeHash = 0,
            VkShaderModuleCreateFlags flags = 0,
            bool reflect = false,
            std::shared_ptr<IAllocator> allocator = nullptr
#       ifdef VK_EXT_validation_cache
            ,std::shared_ptr<ValidationCache> validationCache = nullptr
#       endif
        );
        ~ShaderModule();
        std::size_t getHash() noexcept;
        std::shared_ptr<ShaderReflection> getReflection() const noexcept { return reflection; }

    private:
        std::size_t hash;
        std::vector<char> bytecode;
        std::shared_ptr<ShaderReflection> reflection;
    };
} // namespace magma

#include "shaderModule.inl"
