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
#include "../misc/exception.h"

namespace magma
{
    /* See https://www.khronos.org/registry/spir-v/specs/1.0/SPIRV.pdf
       2.3 Physical Layout of a SPIR-V Module and Instruction */
    typedef uint32_t SpirvWord;

    /* SPIRV-Reflect is a lightweight library that provides a C/C++ reflection API
       for SPIR-V shader bytecode in Vulkan applications.
       https://github.com/chaoticbob/SPIRV-Reflect */

    class ShaderReflection : public core::NonCopyable
    {
    public:
        ShaderReflection(const SpirvWord *bytecode, std::size_t bytecodeSize);
        ~ShaderReflection() noexcept;
        const SpirvWord *getBytecode() const noexcept;
        std::size_t getBytecodeSize() const noexcept;
        uint32_t getEntryPointCount() const noexcept { return module.entry_point_count; }
        const char *getEntryPointName(uint32_t index) const noexcept { return module.entry_points[index].name; }
        VkShaderStageFlagBits getShaderStage() const noexcept { return static_cast<VkShaderStageFlagBits>(module.shader_stage); };
        SpvSourceLanguage getSourceLanguage() const noexcept { return module.source_language; }
        uint32_t getSourceLanguageVersion() const noexcept { return module.source_language_version; }
        const SpvReflectEntryPoint& getEntryPoint(const char *name) const noexcept;
        std::vector<const SpvReflectDescriptorBinding *> enumerateDescriptorBindings() const;
        std::vector<const SpvReflectDescriptorBinding *> enumerateEntryPointDescriptorBindings(const char *name) const;
        std::vector<const SpvReflectDescriptorSet *> enumerateDescriptorSets() const;
        std::vector<const SpvReflectDescriptorSet *> enumerateEntryPointDescriptorSets(const char *name) const;
        std::vector<const SpvReflectInterfaceVariable *> enumerateInputVariables() const;
        std::vector<const SpvReflectInterfaceVariable *> enumerateEntryPointInputVariables(const char *name) const;
        std::vector<const SpvReflectInterfaceVariable *> enumerateOutputVariables() const;
        std::vector<const SpvReflectInterfaceVariable *> enumerateEntryPointOutputVariables(const char *name) const;
        std::vector<const SpvReflectBlockVariable *> enumeratePushConstantBlocks() const;
        std::vector<const SpvReflectBlockVariable *> enumerateEntryPointPushConstantBlocks(const char *name) const;

    private:
        template<typename Type, typename Func>
        std::vector<const Type *> enumerateObjects(Func, const char *) const;
        template<typename Type, typename Func>
        std::vector<const Type *> enumerateEntryPointObjects(Func, const char *, const char *) const;

        SpvReflectShaderModule module;
    };
} // namespace magma

#include "shaderReflection.inl"
