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
#include "pipelineShaderStage.h"
#include "specialization.h"
#include "shaderReflection.h"
#include "../objects/shaderModule.h"
#include "../helpers/stringifyFlags.h"

namespace magma
{
PipelineShaderStage::PipelineShaderStage(std::shared_ptr<ShaderModule> shaderModule,
    std::shared_ptr<Specialization> specialization /* nullptr */,
    VkPipelineShaderStageCreateFlags flags /* 0 */) noexcept:
    VkPipelineShaderStageCreateInfo{
        VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        nullptr,
        flags,
        shaderModule->getStage(),
        *shaderModule,
        core::copyString(shaderModule->getReflection()->getEntryPointName(0)),
        specialization.get()
    },
    shaderModule(std::move(shaderModule)),
    specialization(std::move(specialization))
{}

PipelineShaderStage::PipelineShaderStage(VkShaderStageFlagBits stage, std::shared_ptr<ShaderModule> shaderModule, const char *entrypoint,
    std::shared_ptr<Specialization> specialization /* nullptr */,
    VkPipelineShaderStageCreateFlags flags /* 0 */) noexcept:
    VkPipelineShaderStageCreateInfo{
        VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        nullptr,
        flags,
        stage,
        *shaderModule,
        core::copyString(entrypoint),
        specialization.get()
    },
    shaderModule(std::move(shaderModule)),
    specialization(std::move(specialization))
{}

PipelineShaderStage::PipelineShaderStage(const PipelineShaderStage& other) noexcept:
    VkPipelineShaderStageCreateInfo{
        VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        other.pNext,
        other.flags,
        other.stage,
        other.module,
        core::copyString(other.pName),
        other.pSpecializationInfo,
    },
    shaderModule(other.shaderModule),
    specialization(other.specialization)
{}

PipelineShaderStage::PipelineShaderStage(PipelineShaderStage&& other) noexcept:
    VkPipelineShaderStageCreateInfo{
        VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        other.pNext,
        other.flags,
        other.stage,
        other.module,
        other.pName,
        other.pSpecializationInfo,
    },
    shaderModule(std::move(other.shaderModule)),
    specialization(std::move(other.specialization))
{
    other.pNext = nullptr;
    other.flags = 0;
    other.stage = (VkShaderStageFlagBits)0;
    other.module = VK_NULL_HANDLE;
    other.pName = nullptr;
    other.pSpecializationInfo = nullptr;
}

PipelineShaderStage& PipelineShaderStage::operator=(const PipelineShaderStage& other) noexcept
{
    if (this != &other)
    {
        this->~PipelineShaderStage();
        shaderModule = other.shaderModule;
        specialization = other.specialization;
        flags = other.flags;
        stage = other.stage;
        module = other.module;
        pName = core::copyString(other.pName);
        pSpecializationInfo = specialization.get();
    }
    return *this;
}

PipelineShaderStage& PipelineShaderStage::operator=(PipelineShaderStage&& other) noexcept
{
    if (this != &other)
    {
        this->~PipelineShaderStage();
        pNext = other.pNext;
        flags = other.flags;
        stage = other.stage;
        module = other.module;
        pName = other.pName;
        pSpecializationInfo = other.pSpecializationInfo,
        shaderModule = std::move(other.shaderModule);
        specialization = std::move(other.specialization);
        other.pNext = nullptr;
        other.flags = 0;
        other.stage = (VkShaderStageFlagBits)0;
        other.module = VK_NULL_HANDLE;
        other.pName = nullptr;
        other.pSpecializationInfo = nullptr;
    }
    return *this;
}

hash_t PipelineShaderStage::getHash() const noexcept
{   // Compute complex hash on demand
    hash_t hash = core::hashArgs(
        sType,
        flags,
        stage,
        module);
    hash = core::hashCombine(hash, shaderModule->getHash());
    if (pName)
        hash = core::hashCombine(hash, core::hashString(pName));
    if (specialization)
        hash = core::hashCombine(hash, specialization->getHash());
    return hash;
}

std::ostream& operator<<(std::ostream& out, const PipelineShaderStage& shaderStage)
{
    out << "VkPipelineShaderStageCreateInfo [" << std::endl
        << "\tflags: " << helpers::stringifyPipelineShaderStageFlags(shaderStage.flags) << std::endl
        << "\tstage: " << helpers::stringifyShaderStageFlags(shaderStage.stage) << std::endl
        << "\tmodule: 0x" << std::hex << shaderStage.module << std::dec << std::endl
        << "\tpName: " << shaderStage.pName << std::endl;
    if (!shaderStage.pSpecializationInfo)
        out << "\tpSpecializationInfo: NULL" << std::endl;
    else
        out << *shaderStage.specialization << std::endl;
    out << "]";
    const std::string disassembly = shaderStage.shaderModule->disassemble();
    if (!disassembly.empty())
        out << std::endl << "Disassembly [" << std::endl << disassembly << "]";
    auto const& reflection = shaderStage.shaderModule->getReflection();
    if (reflection)
        out << std::endl << *reflection;
    return out;
}
} // namespace magma
