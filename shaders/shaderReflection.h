/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2022 Victor Coda.

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
#include "../third-party/SPIRV-Reflect/spirv_reflect.h"

namespace magma
{
    /* See https://www.khronos.org/registry/spir-v/specs/1.0/SPIRV.pdf
       2.3 Physical Layout of a SPIR-V Module and Instruction */
    typedef uint32_t SpirvWord;

    /* https://github.com/chaoticbob/SPIRV-Reflect
       SPIRV-Reflect is a lightweight library that provides a C/C++ reflection API
       for SPIR-V shader bytecode in Vulkan applications. */

    class ShaderReflection : public core::NonCopyable
    {
    public:
        explicit ShaderReflection(const SpirvWord *bytecode, std::size_t bytecodeSize);
        ~ShaderReflection() noexcept;
        const SpirvWord *getBytecode() const noexcept;
        std::size_t getBytecodeSize() const noexcept;
        std::size_t computeBytecodeHash() const noexcept;
        uint32_t getEntryPointCount() const noexcept { return module.entry_point_count; }
        const char *getEntryPointName(uint32_t index) const noexcept { return module.entry_points[index].name; }
        const SpvReflectEntryPoint& getEntryPoint(const char *name) const noexcept;
        VkShaderStageFlagBits getShaderStage() const noexcept { return static_cast<VkShaderStageFlagBits>(module.shader_stage); };
        SpvSourceLanguage getSourceLanguage() const noexcept { return module.source_language; }
        uint32_t getSourceLanguageVersion() const noexcept { return module.source_language_version; }
        SpvExecutionModel getExecutionModel() const noexcept { return module.spirv_execution_model; }
        std::vector<const SpvReflectDescriptorBinding *> enumerateDescriptorBindings(const char *entrypoint = nullptr) const;
        std::vector<const SpvReflectDescriptorSet *> enumerateDescriptorSets(const char *entrypoint = nullptr) const;
        std::vector<const SpvReflectInterfaceVariable *> enumerateInputVariables(const char *entrypoint = nullptr) const;
        std::vector<const SpvReflectInterfaceVariable *> enumerateOutputVariables(const char *entrypoint = nullptr) const;
        std::vector<const SpvReflectBlockVariable *> enumeratePushConstantBlocks(const char *entrypoint = nullptr) const;
        const SpvReflectDescriptorBinding *getDescriptorBinding(const char *entrypoint, uint32_t binding, uint32_t set) const;
        const SpvReflectDescriptorSet *getDescriptorSet(const char *entrypoint, uint32_t set) const;
        const SpvReflectInterfaceVariable *getInputVariable(const char *entrypoint, uint32_t location) const;
        const SpvReflectInterfaceVariable *getInputVariable(const char *entrypoint, const char *semantic) const;
        const SpvReflectInterfaceVariable *getOutputVariable(const char *entrypoint, uint32_t location) const;
        const SpvReflectInterfaceVariable *getOutputVariable(const char *entrypoint, const char *semantic) const;
        const SpvReflectBlockVariable *getPushConstantBlock(const uint32_t index) const;
        const SpvReflectBlockVariable *getPushConstantBlock(const char *entrypoint) const;

    private:
        SpvReflectShaderModule module;
    };
} // namespace magma
