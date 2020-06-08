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
#include "../exceptions/reflectException.h"

namespace magma
{
ShaderReflection::ShaderReflection(const SpirvWord *bytecode, const std::size_t bytecodeSize)
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

std::size_t ShaderReflection::computeBytecodeHash() const noexcept
{
    const std::size_t wordCount = getBytecodeSize() / sizeof(SpirvWord);
    return core::hashArray(getBytecode(), wordCount);
}

const SpvReflectEntryPoint& ShaderReflection::getEntryPoint(const char *name) const noexcept
{
    return *spvReflectGetEntryPoint(&module, name);
}

std::vector<const SpvReflectDescriptorBinding *> ShaderReflection::enumerateDescriptorBindings(const char *entrypoint /* nullptr */) const
{
    std::vector<const SpvReflectDescriptorBinding *> descriptorBindings;
    uint32_t count = 0;
    entrypoint ? spvReflectEnumerateEntryPointDescriptorBindings(&module, entrypoint, &count, nullptr)
               : spvReflectEnumerateDescriptorBindings(&module, &count, nullptr);
    if (count)
    {
        descriptorBindings.resize(count);
        auto data = const_cast<SpvReflectDescriptorBinding **>(descriptorBindings.data());
        const SpvReflectResult result = entrypoint ?
            spvReflectEnumerateEntryPointDescriptorBindings(&module, entrypoint, &count, data) :
            spvReflectEnumerateDescriptorBindings(&module, &count, data);
        MAGMA_THROW_REFLECTION_FAILURE(result, "failed to enumerate descriptor bindings")
    }
    return descriptorBindings;
}

std::vector<const SpvReflectDescriptorSet *> ShaderReflection::enumerateDescriptorSets(const char *entrypoint /* nullptr */) const
{
    std::vector<const SpvReflectDescriptorSet *> descriptorSets;
    uint32_t count = 0;
    entrypoint ? spvReflectEnumerateEntryPointDescriptorSets(&module, entrypoint, &count, nullptr)
               : spvReflectEnumerateDescriptorSets(&module, &count, nullptr);
    if (count)
    {
        descriptorSets.resize(count);
        auto data = const_cast<SpvReflectDescriptorSet **>(descriptorSets.data());
        const SpvReflectResult result = entrypoint ?
            spvReflectEnumerateEntryPointDescriptorSets(&module, entrypoint, &count, data) :
            spvReflectEnumerateDescriptorSets(&module, &count, data);
        MAGMA_THROW_REFLECTION_FAILURE(result, "failed to enumerate descriptor sets")
    }
    return descriptorSets;
}

std::vector<const SpvReflectInterfaceVariable *> ShaderReflection::enumerateInputVariables(const char *entrypoint /* nullptr */) const
{
    std::vector<const SpvReflectInterfaceVariable *> inputVariables;
    uint32_t count = 0;
    entrypoint ? spvReflectEnumerateEntryPointInputVariables(&module, entrypoint, &count, nullptr)
               : spvReflectEnumerateInputVariables(&module, &count, nullptr);
    if (count)
    {
        inputVariables.resize(count);
        auto data = const_cast<SpvReflectInterfaceVariable **>(inputVariables.data());
        const SpvReflectResult result = entrypoint ?
            spvReflectEnumerateEntryPointInputVariables(&module, entrypoint, &count, data) :
            spvReflectEnumerateInputVariables(&module, &count, data);
        MAGMA_THROW_REFLECTION_FAILURE(result, "failed to enumerate input variables")
    }
    return inputVariables;
}


std::vector<const SpvReflectInterfaceVariable *> ShaderReflection::enumerateOutputVariables(const char *entrypoint /* nullptr */) const
{
    std::vector<const SpvReflectInterfaceVariable *> outputVariables;
    uint32_t count = 0;
    entrypoint ? spvReflectEnumerateEntryPointOutputVariables(&module, entrypoint, &count, nullptr)
               : spvReflectEnumerateOutputVariables(&module, &count, nullptr);
    if (count)
    {
        outputVariables.resize(count);
        auto data = const_cast<SpvReflectInterfaceVariable **>(outputVariables.data());
        const SpvReflectResult result = entrypoint ?
            spvReflectEnumerateEntryPointOutputVariables(&module, entrypoint, &count, data) :
            spvReflectEnumerateOutputVariables(&module, &count, data);
        MAGMA_THROW_REFLECTION_FAILURE(result, "failed to enumerate output variables")
    }
    return outputVariables;
}


std::vector<const SpvReflectBlockVariable *> ShaderReflection::enumeratePushConstantBlocks(const char *entrypoint /* nullptr */) const
{
    std::vector<const SpvReflectBlockVariable *> pushConstantBlocks;
    uint32_t count = 0;
    entrypoint ? spvReflectEnumerateEntryPointPushConstantBlocks(&module, entrypoint, &count, nullptr)
               : spvReflectEnumeratePushConstantBlocks(&module, &count, nullptr);
    if (count)
    {
        pushConstantBlocks.resize(count);
        auto data = const_cast<SpvReflectBlockVariable **>(pushConstantBlocks.data());
        const SpvReflectResult result = entrypoint ?
            spvReflectEnumerateEntryPointPushConstantBlocks(&module, entrypoint, &count, data) :
            spvReflectEnumeratePushConstantBlocks(&module, &count, data);
        MAGMA_THROW_REFLECTION_FAILURE(result, "failed to enumerate push constant blocks")
    }
    return pushConstantBlocks;
}

const SpvReflectDescriptorBinding* ShaderReflection::getDescriptorBinding(const char *entrypoint, const uint32_t binding, const uint32_t set) const
{
    SpvReflectResult result;
    const SpvReflectDescriptorBinding *descriptorBinding = entrypoint ?
        spvReflectGetEntryPointDescriptorBinding(&module, entrypoint, binding, set, &result) :
        spvReflectGetDescriptorBinding(&module, binding, set, &result);
    MAGMA_THROW_REFLECTION_FAILURE(result, "failed to get descriptor binding")
    return descriptorBinding;
}

const SpvReflectDescriptorSet *ShaderReflection::getDescriptorSet(const char *entrypoint, const uint32_t set) const
{
    SpvReflectResult result;
    const SpvReflectDescriptorSet *descriptorSet = entrypoint ?
        spvReflectGetEntryPointDescriptorSet(&module, entrypoint, set, &result) :
        spvReflectGetDescriptorSet(&module, set, &result);
    MAGMA_THROW_REFLECTION_FAILURE(result, "failed to get descriptor set")
    return descriptorSet;
}

const SpvReflectInterfaceVariable* ShaderReflection::getInputVariable(const char *entrypoint, const uint32_t location) const
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

const SpvReflectInterfaceVariable *ShaderReflection::getOutputVariable(const char *entrypoint, const uint32_t location) const
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

const SpvReflectBlockVariable *ShaderReflection::getPushConstantBlock(const uint32_t index) const
{
    SpvReflectResult result;
    const SpvReflectBlockVariable *pushConstantBlock = spvReflectGetPushConstantBlock(&module, index, &result);
    MAGMA_THROW_REFLECTION_FAILURE(result, "failed to get push constant block by index")
    return pushConstantBlock;
}

const SpvReflectBlockVariable *ShaderReflection::getPushConstantBlock(const char *entrypoint) const
{
    MAGMA_ASSERT(entrypoint);
    SpvReflectResult result;
    const SpvReflectBlockVariable *pushConstantBlock = spvReflectGetEntryPointPushConstantBlock(&module, entrypoint, &result);
    MAGMA_THROW_REFLECTION_FAILURE(result, "failed to get push constant block for entry point")
    return pushConstantBlock;
}
} // namespace magma
