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
#include "pch.h"
#include "shaderReflection.h"
#include "../misc/exception.h"

namespace magma
{
ShaderReflection::ShaderReflection(const SpirvWord *bytecode, std::size_t bytecodeSize)
{
    MAGMA_ASSERT(0 == bytecodeSize % sizeof(SpirvWord)); // A module is defined as a stream of words, not a stream of bytes
    const SpvReflectResult result = spvReflectCreateShaderModule(bytecodeSize, bytecode, &module);
    MAGMA_THROW_REFLECTION_FAILURE(result, "failed to create shader reflection")
}

ShaderReflection::~ShaderReflection() noexcept
{
    spvReflectDestroyShaderModule(&module);
}

const SpirvWord *ShaderReflection::getBytecode() const noexcept
{
    return spvReflectGetCode(&module);
}

std::size_t ShaderReflection::getBytecodeSize() const noexcept
{
    const std::size_t bytecodeSize = spvReflectGetCodeSize(&module);
    MAGMA_ASSERT(0 == bytecodeSize % sizeof(SpirvWord));
    return bytecodeSize;
}

const SpvReflectEntryPoint& ShaderReflection::getEntryPoint(const char *name) const noexcept
{
    return *spvReflectGetEntryPoint(&module, name);
}

std::vector<const SpvReflectDescriptorBinding *> ShaderReflection::enumerateDescriptorBindings() const
{
    return enumerateObjects<SpvReflectDescriptorBinding>(
        spvReflectEnumerateDescriptorBindings,
        "failed to enumerate descriptor bindings");
}

std::vector<const SpvReflectDescriptorBinding *> ShaderReflection::enumerateEntryPointDescriptorBindings(const char *name) const
{
     return enumerateEntryPointObjects<SpvReflectDescriptorBinding>(
         spvReflectEnumerateEntryPointDescriptorBindings,
         name,
         "failed to enumerate entry point descriptor bindings");
}

std::vector<const SpvReflectDescriptorSet *> ShaderReflection::enumerateDescriptorSets() const
{
    return enumerateObjects<SpvReflectDescriptorSet>(
        spvReflectEnumerateDescriptorSets,
        "failed to enumerate descriptor sets");
}

std::vector<const SpvReflectDescriptorSet *> ShaderReflection::enumerateEntryPointDescriptorSets(const char *name) const
{
    return enumerateEntryPointObjects<SpvReflectDescriptorSet>(
         spvReflectEnumerateEntryPointDescriptorSets,
         name,
         "failed to enumerate entry point descriptor sets");
}

std::vector<const SpvReflectInterfaceVariable *> ShaderReflection::enumerateInputVariables() const
{
    return enumerateObjects<SpvReflectInterfaceVariable>(
        spvReflectEnumerateInputVariables,
        "failed to enumerate input variables");
}

std::vector<const SpvReflectInterfaceVariable *> ShaderReflection::enumerateEntryPointInputVariables(const char *name) const
{
    return enumerateEntryPointObjects<SpvReflectInterfaceVariable>(
        spvReflectEnumerateEntryPointInputVariables,
        name,
        "failed to enumerate entry point input variables");
}

std::vector<const SpvReflectInterfaceVariable *> ShaderReflection::enumerateOutputVariables() const
{
    return enumerateObjects<SpvReflectInterfaceVariable>(
        spvReflectEnumerateOutputVariables,
        "failed to enumerate output variables");
}

std::vector<const SpvReflectInterfaceVariable *> ShaderReflection::enumerateEntryPointOutputVariables(const char *name) const
{
    return enumerateEntryPointObjects<SpvReflectInterfaceVariable>(
        spvReflectEnumerateEntryPointOutputVariables,
        name,
        "failed to enumerate entry point output variables");
}

std::vector<const SpvReflectBlockVariable *> ShaderReflection::enumeratePushConstantBlocks() const
{
    return enumerateObjects<SpvReflectBlockVariable>(
        spvReflectEnumeratePushConstantBlocks,
        "failed to enumerate push constant blocks");
}

std::vector<const SpvReflectBlockVariable *> ShaderReflection::enumerateEntryPointPushConstantBlocks(const char *name) const
{
    return enumerateEntryPointObjects<SpvReflectBlockVariable>(
        spvReflectEnumerateEntryPointPushConstantBlocks,
        name,
        "failed to enumerate entry point push constant blocks");
}

const SpvReflectDescriptorBinding* ShaderReflection::getDescriptorBinding(const char *entrypoint, uint32_t binding, uint32_t set) const
{
    SpvReflectResult result;
    const SpvReflectDescriptorBinding *descriptorBinding = entrypoint ?
        spvReflectGetEntryPointDescriptorBinding(&module, entrypoint, binding, set, &result) :
        spvReflectGetDescriptorBinding(&module, binding, set, &result);
    MAGMA_THROW_REFLECTION_FAILURE(result, "failed to get descriptor binding");
    return descriptorBinding;
}

const SpvReflectDescriptorSet *ShaderReflection::getDescriptorSet(const char *entrypoint, uint32_t set) const
{
    SpvReflectResult result;
    const SpvReflectDescriptorSet *descriptorSet = entrypoint ?
        spvReflectGetEntryPointDescriptorSet(&module, entrypoint, set, &result) :
        spvReflectGetDescriptorSet(&module, set, &result);
    MAGMA_THROW_REFLECTION_FAILURE(result, "failed to get descriptor set");
    return descriptorSet;
}

const SpvReflectInterfaceVariable* ShaderReflection::getInputVariable(const char *entrypoint, uint32_t location) const
{
    SpvReflectResult result;
    const SpvReflectInterfaceVariable* inputVariable = entrypoint ?
        spvReflectGetEntryPointInputVariableByLocation(&module, entrypoint, location, &result) :
        spvReflectGetInputVariableByLocation(&module, location, &result);
    MAGMA_THROW_REFLECTION_FAILURE(result, "failed to get input variable by location")
    return inputVariable;
}

const SpvReflectInterfaceVariable *ShaderReflection::getInputVariable(const char *entrypoint, const char *semantic) const
{
    SpvReflectResult result;
    const SpvReflectInterfaceVariable* inputVariable = entrypoint ?
        spvReflectGetEntryPointInputVariableBySemantic(&module, entrypoint, semantic, &result) :
        spvReflectGetInputVariableBySemantic(&module, semantic, &result);
    MAGMA_THROW_REFLECTION_FAILURE(result, "failed to get input variable by semantic")
    return inputVariable;
}

const SpvReflectInterfaceVariable *ShaderReflection::getOutputVariable(const char *entrypoint, uint32_t location) const
{
    SpvReflectResult result;
    const SpvReflectInterfaceVariable* outputVariable = entrypoint ?
        spvReflectGetEntryPointOutputVariableByLocation(&module, entrypoint, location, &result) :
        spvReflectGetOutputVariableByLocation(&module, location, &result);
    MAGMA_THROW_REFLECTION_FAILURE(result, "failed to get output variable by location")
    return outputVariable;
}

const SpvReflectInterfaceVariable *ShaderReflection::getOutputVariable(const char *entrypoint, const char *semantic) const
{
    SpvReflectResult result;
    const SpvReflectInterfaceVariable* outputVariable = entrypoint ?
        spvReflectGetEntryPointOutputVariableBySemantic(&module, entrypoint, semantic, &result) :
        spvReflectGetOutputVariableBySemantic(&module, semantic, &result);
    MAGMA_THROW_REFLECTION_FAILURE(result, "failed to get output variable by semantic")
    return outputVariable;
}
} // namespace magma
