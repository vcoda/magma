/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2025 Victor Coda.

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
#include "model/nondispatchable.h"

namespace magma
{
    class Device;
    class ValidationCache;
    class ShaderReflection;
    class IAllocator;

    /* See https://www.khronos.org/registry/spir-v/specs/1.0/SPIRV.pdf
       2.3 Physical Layout of a SPIR-V Module and Instruction */
    typedef uint32_t SpirvWord;

    /* Shader modules contain shader code and one or more
       entry points. Shaders are selected from a shader module
       by specifying an entry point. The stages of a pipeline
       can use shaders that come from different modules. The
       shader code defining a shader module must be in the
       SPIR-V format. */

    class ShaderModule : public NonDispatchable<VkShaderModule>
    {
    public:
        explicit ShaderModule(std::shared_ptr<Device> device,
            const SpirvWord *bytecode,
            std::size_t bytecodeSize,
            hash_t bytecodeHash = 0,
            std::shared_ptr<IAllocator> allocator = nullptr,
            bool reflect = false,
            VkShaderModuleCreateFlags flags = 0,
        #ifdef VK_EXT_validation_cache
            lent_ptr<ValidationCache> validationCache = nullptr,
        #endif
            const StructureChain& extendedInfo = StructureChain());
        explicit ShaderModule(std::shared_ptr<Device> device,
            const std::vector<SpirvWord>& bytecode,
            hash_t bytecodeHash = 0,
            std::shared_ptr<IAllocator> allocator = nullptr,
            bool reflect = false,
            VkShaderModuleCreateFlags flags = 0,
        #ifdef VK_EXT_validation_cache
            lent_ptr<ValidationCache> validationCache = nullptr,
        #endif
            const StructureChain& extendedInfo = StructureChain());
        template<std::size_t SpirvWordCount>
        explicit ShaderModule(std::shared_ptr<Device> device,
            const SpirvWord (&bytecode)[SpirvWordCount],
            hash_t bytecodeHash = 0,
            std::shared_ptr<IAllocator> allocator = nullptr,
            bool reflect = false,
            VkShaderModuleCreateFlags flags = 0,
        #ifdef VK_EXT_validation_cache
            lent_ptr<ValidationCache> validationCache = nullptr,
        #endif
            const StructureChain& extendedInfo = StructureChain());
        ~ShaderModule();
        VkShaderStageFlagBits getStage() const noexcept;
        const std::unique_ptr<const ShaderReflection>& getReflection() const noexcept { return reflection; }
        hash_t getHash() const noexcept;
        hash_t getBytecodeHash() const noexcept;
        std::string disassemble() const;

    private:
        std::unique_ptr<const ShaderReflection> reflection;
        hash_t hash;
        mutable hash_t bytecodeHash;
        mutable std::vector<SpirvWord> bytecode;
    };
} // namespace magma

#include "shaderModule.inl"
