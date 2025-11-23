/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2024 Victor Coda.

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
#pragma hdrstop
#include "shaderReflection.h"
#include "../exceptions/reflectionError.h"
#include "../helpers/streamInsertOperators.h"
#include "../helpers/stringifyFlags.h"

namespace magma
{
ShaderReflection::ShaderReflection(const SpirvWord *bytecode, std::size_t bytecodeSize)
{   // A module is defined as a stream of words, not a stream of bytes
    MAGMA_ASSERT(0 == bytecodeSize % sizeof(SpirvWord));
    const SpvReflectResult result = spvReflectCreateShaderModule(bytecodeSize, bytecode, &module);
    MAGMA_HANDLE_REFLECTION_RESULT(result, "failed to create shader reflection");
}

ShaderReflection::~ShaderReflection()
{
    spvReflectDestroyShaderModule(&module);
}

const SpirvWord *ShaderReflection::getCode() const noexcept
{
    return spvReflectGetCode(&module);
}

std::size_t ShaderReflection::getCodeSize() const noexcept
{
    return spvReflectGetCodeSize(&module);
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
        MAGMA_HANDLE_REFLECTION_RESULT(result, "failed to enumerate descriptor bindings");
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
        MAGMA_HANDLE_REFLECTION_RESULT(result, "failed to enumerate descriptor sets");
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
        const SpvReflectResult result = entrypoint
            ? spvReflectEnumerateEntryPointInputVariables(&module, entrypoint, &count, data)
            : spvReflectEnumerateInputVariables(&module, &count, data);
        MAGMA_HANDLE_REFLECTION_RESULT(result, "failed to enumerate input variables");
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
        const SpvReflectResult result = entrypoint
            ? spvReflectEnumerateEntryPointOutputVariables(&module, entrypoint, &count, data)
            : spvReflectEnumerateOutputVariables(&module, &count, data);
        MAGMA_HANDLE_REFLECTION_RESULT(result, "failed to enumerate output variables");
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
        const SpvReflectResult result = entrypoint
            ? spvReflectEnumerateEntryPointPushConstantBlocks(&module, entrypoint, &count, data)
            : spvReflectEnumeratePushConstantBlocks(&module, &count, data);
        MAGMA_HANDLE_REFLECTION_RESULT(result, "failed to enumerate push constant blocks");
    }
    return pushConstantBlocks;
}

const SpvReflectDescriptorBinding* ShaderReflection::getDescriptorBinding(const char *entrypoint, uint32_t binding, uint32_t set) const
{
    SpvReflectResult result;
    const SpvReflectDescriptorBinding *descriptorBinding = entrypoint
        ? spvReflectGetEntryPointDescriptorBinding(&module, entrypoint, binding, set, &result)
        : spvReflectGetDescriptorBinding(&module, binding, set, &result);
    MAGMA_HANDLE_REFLECTION_RESULT(result, "failed to get descriptor binding");
    return descriptorBinding;
}

const SpvReflectDescriptorSet *ShaderReflection::getDescriptorSet(const char *entrypoint, uint32_t set) const
{
    SpvReflectResult result;
    const SpvReflectDescriptorSet *descriptorSet = entrypoint
        ? spvReflectGetEntryPointDescriptorSet(&module, entrypoint, set, &result)
        : spvReflectGetDescriptorSet(&module, set, &result);
    MAGMA_HANDLE_REFLECTION_RESULT(result, "failed to get descriptor set");
    return descriptorSet;
}

const SpvReflectInterfaceVariable* ShaderReflection::getInputVariable(const char *entrypoint, uint32_t location) const
{
    SpvReflectResult result;
    const SpvReflectInterfaceVariable* inputVariable = entrypoint
        ? spvReflectGetEntryPointInputVariableByLocation(&module, entrypoint, location, &result)
        : spvReflectGetInputVariableByLocation(&module, location, &result);
    MAGMA_HANDLE_REFLECTION_RESULT(result, "failed to get input variable by location");
    return inputVariable;
}

const SpvReflectInterfaceVariable *ShaderReflection::getInputVariable(const char *entrypoint, const char *semantic) const
{
    SpvReflectResult result;
    const SpvReflectInterfaceVariable* inputVariable = entrypoint
        ? spvReflectGetEntryPointInputVariableBySemantic(&module, entrypoint, semantic, &result)
        : spvReflectGetInputVariableBySemantic(&module, semantic, &result);
    MAGMA_HANDLE_REFLECTION_RESULT(result, "failed to get input variable by semantic");
    return inputVariable;
}

const SpvReflectInterfaceVariable *ShaderReflection::getOutputVariable(const char *entrypoint, uint32_t location) const
{
    SpvReflectResult result;
    const SpvReflectInterfaceVariable* outputVariable = entrypoint
        ? spvReflectGetEntryPointOutputVariableByLocation(&module, entrypoint, location, &result)
        : spvReflectGetOutputVariableByLocation(&module, location, &result);
    MAGMA_HANDLE_REFLECTION_RESULT(result, "failed to get output variable by location");
    return outputVariable;
}

const SpvReflectInterfaceVariable *ShaderReflection::getOutputVariable(const char *entrypoint, const char *semantic) const
{
    SpvReflectResult result;
    const SpvReflectInterfaceVariable* outputVariable = entrypoint
        ? spvReflectGetEntryPointOutputVariableBySemantic(&module, entrypoint, semantic, &result)
        : spvReflectGetOutputVariableBySemantic(&module, semantic, &result);
    MAGMA_HANDLE_REFLECTION_RESULT(result, "failed to get output variable by semantic");
    return outputVariable;
}

const SpvReflectBlockVariable *ShaderReflection::getPushConstantBlock(uint32_t index) const
{
    SpvReflectResult result;
    const SpvReflectBlockVariable *pushConstantBlock = spvReflectGetPushConstantBlock(&module, index, &result);
    MAGMA_HANDLE_REFLECTION_RESULT(result, "failed to get push constant block by index");
    return pushConstantBlock;
}

const SpvReflectBlockVariable *ShaderReflection::getPushConstantBlock(const char *entrypoint) const
{
    MAGMA_ASSERT(entrypoint);
    SpvReflectResult result;
    const SpvReflectBlockVariable *pushConstantBlock = spvReflectGetEntryPointPushConstantBlock(&module, entrypoint, &result);
    MAGMA_HANDLE_REFLECTION_RESULT(result, "failed to get push constant block for entry point");
    return pushConstantBlock;
}

std::ostream& operator<<(std::ostream& out, const ShaderReflection& reflection)
{
    const SpvReflectShaderModule& module = reflection.module;
    out << "SpvReflectShaderModule [" << std::endl
        << "\tgenerator: " << module.generator << std::endl
        << "\tshader_stage: " << module.shader_stage << std::endl
        << "\tspirv_size: " << spvReflectGetCodeSize(&module) << std::endl
        << "\tsource_language: " << spvReflectSourceLanguage(module.source_language)
        << " " << module.source_language_version << std::endl;
    for (uint32_t i = 0; i < module.entry_point_count; ++i)
        out << "\tentry_point #" << i << ": \"" << module.entry_points[i].name << "\"" << std::endl;
    out << "\tdescriptor_set_count: " << module.descriptor_set_count << std::endl;
    for (uint32_t i = 0; i < module.descriptor_set_count; ++i)
    {
        const SpvReflectDescriptorSet& set = module.descriptor_sets[i];
        out << "\tSpvReflectDescriptorSet [" << std::endl
            << "\t\tset: " << set.set << std::endl
            << "\t\tbinding_count: " << set.binding_count << std::endl
            << "\t\tbindings: " << std::endl;
        for (uint32_t j = 0; j < set.binding_count; ++j)
        {
            const SpvReflectDescriptorBinding *binding = set.bindings[j];
            out << "\t\t[" << std::endl
                << "\t\t\tbinding: " << binding->binding << std::endl
                << "\t\t\tname: ";
            if (strlen(binding->name))
                out << binding->name << std::endl;
            else if (binding->type_description->type_name && strlen(binding->type_description->type_name))
                out << binding->type_description->type_name << std::endl;
            else
                out << "<none>" << std::endl;
            out << "\t\t\tdescriptor_type: " << binding->descriptor_type << std::endl;
            if (binding->count > 1)
                out << "\t\t\tcount: " << binding->count << std::endl;
            out << "\t\t]" << std::endl;
            // TODO: uav_counter_binding
        }
        out << "\t]" << std::endl;
    }
    out << "\tinput_variable_count: " << module.input_variable_count << std::endl;
    for (uint32_t i = 0; i < module.input_variable_count; ++i)
    {
        const SpvReflectInterfaceVariable *var = module.input_variables[i];
        out << "\tSpvReflectInterfaceVariable [" << std::endl
            << "\t\tname: " << var->name << std::endl
            << "\t\tlocation: " << var->location << std::endl
            << "\t\tformat: " << var->format << std::endl
            << "\t\ttype_flags: " << magma::helpers::stringifyReflectTypeFlags(var->type_description->type_flags) << std::endl;
        out << "\t]" << std::endl;
    }
    out << "\toutput_variable_count: " << module.output_variable_count << std::endl;
    for (uint32_t i = 0; i < module.output_variable_count; ++i)
    {
        const SpvReflectInterfaceVariable *var = module.output_variables[i];
        if ((var->location != 0xFFFFFFFF) && (var->format != SPV_REFLECT_FORMAT_UNDEFINED))
        {
            out << "\tSpvReflectInterfaceVariable [" << std::endl
                << "\t\tname: " << var->name << std::endl
                << "\t\tlocation: " << var->location << std::endl
                << "\t\tformat: " << var->format << std::endl
                << "\t\ttype_flags: " << magma::helpers::stringifyReflectTypeFlags(var->type_description->type_flags) << std::endl;
            out << "\t]" << std::endl;
        }
    }
    out << "]";
    return out;
}
} // namespace magma
