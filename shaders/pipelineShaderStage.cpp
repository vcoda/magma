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
#include "pch.h"
#pragma hdrstop
#include "pipelineShaderStage.h"
#include "../objects/shaderModule.h"

namespace magma
{
PipelineShaderStage::PipelineShaderStage(const VkShaderStageFlagBits stage, std::shared_ptr<ShaderModule> shaderModule, const char *const entrypoint,
    std::shared_ptr<Specialization> specialization /* nullptr */,
    VkPipelineShaderStageCreateFlags flags /* 0 */) noexcept:
    shaderModule(std::move(shaderModule)),
    specialization(std::move(specialization))
{
    sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    pNext = nullptr;
    this->flags = flags;
    this->stage = stage;
    module = MAGMA_HANDLE(shaderModule);
    pName = detail::copyString(entrypoint);
    pSpecializationInfo = specialization.get();
}

PipelineShaderStage::PipelineShaderStage(const PipelineShaderStage& other) noexcept:
    shaderModule(other.shaderModule),
    specialization(other.specialization)
{
    sType = other.sType;
    pNext = other.pNext;
    flags = other.flags;
    stage = other.stage;
    module = other.module;
    pName = detail::copyString(other.pName);
    pSpecializationInfo = specialization.get();
}

PipelineShaderStage& PipelineShaderStage::operator=(const PipelineShaderStage& other) noexcept
{
    if (this != &other)
    {
        shaderModule = other.shaderModule;
        specialization = other.specialization;
        sType = other.sType;
        pNext = other.pNext;
        flags = other.flags;
        stage = other.stage;
        module = other.module;
        delete[] pName;
        pName = detail::copyString(other.pName);
        pSpecializationInfo = specialization.get();
    }
    return *this;
}

PipelineShaderStage::~PipelineShaderStage()
{
    delete[] pName;
}

std::size_t PipelineShaderStage::getHash() const noexcept
{   // Compute complex hash on demand
    std::size_t hash = detail::hashArgs(
        sType,
        flags,
        stage,
        module);
    detail::hashCombine(hash, shaderModule->getHash());
    detail::hashCombine(hash, detail::hashString(std::string(pName)));
    if (specialization)
        detail::hashCombine(hash, specialization->hash());
    return hash;
}
} // namespace magma
