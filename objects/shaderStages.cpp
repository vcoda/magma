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
#include "shaderStages.h"
#include "shaderModule.h"
#include "../internal/hash.h"

namespace magma
{
Specialization::Specialization(const Specialization& other)
{
    mapEntryCount = other.mapEntryCount;
    pMapEntries = internal::copyArray(other.pMapEntries, mapEntryCount);
    dataSize = other.dataSize;
    try {
        pData = internal::copyArray(reinterpret_cast<const char *>(other.pData), dataSize);
    } catch (const std::bad_alloc& exc)
    {
        delete[] pMapEntries;
        throw exc;
    }
}

Specialization& Specialization::operator=(const Specialization& other)
{
    if (this != &other)
    {
        mapEntryCount = other.mapEntryCount;
        pMapEntries = internal::copyArray(other.pMapEntries, mapEntryCount);
        dataSize = other.dataSize;
        try {
            pData = internal::copyArray(reinterpret_cast<const char *>(other.pData), dataSize);
        } catch (const std::bad_alloc& exc)
        {
            delete[] pMapEntries;
            throw exc;
        }
    }
    return *this;
}

Specialization::~Specialization()
{
    delete[] pMapEntries;
    delete[] reinterpret_cast<const char *>(pData);
}

size_t Specialization::hash() const noexcept
{
    size_t hash = 0;
    for (uint32_t i = 0; i < mapEntryCount; ++i)
    {
        internal::hashCombine(hash, internal::hashArgs(
            pMapEntries[i].constantID,
            pMapEntries[i].offset,
            pMapEntries[i].size));
    }
    internal::hashCombine(hash, internal::hashArray(
        reinterpret_cast<const uint8_t *>(pData), dataSize));
    return hash;
}

PipelineShaderStage::PipelineShaderStage(const VkShaderStageFlagBits stage, std::shared_ptr<const ShaderModule> module, const char *const entrypoint,
    std::shared_ptr<const Specialization> specialization, VkPipelineShaderStageCreateFlags flags) noexcept:
    module(std::move(module)),
    specialization(std::move(specialization))
{
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = flags;
    info.stage = stage;
    info.module = *this->module;
    info.pName = internal::copyString(entrypoint);
    info.pSpecializationInfo = this->specialization.get();
}

PipelineShaderStage::PipelineShaderStage(const PipelineShaderStage& other):
    info(other.info),
    module(other.module),
    specialization(other.specialization)
{
    info.pName = internal::copyString(other.info.pName);
}

PipelineShaderStage& PipelineShaderStage::operator=(const PipelineShaderStage& other)
{
    if (this != &other)
    {
        info = other.info;
        info.pName = internal::copyString(other.info.pName);
        module = other.module;
        specialization = other.specialization;
    }
    return *this;
}

PipelineShaderStage::~PipelineShaderStage()
{
    delete[] info.pName;
}

size_t PipelineShaderStage::hash() const noexcept
{
    size_t hash = internal::hashArgs(
        info.flags,
        info.stage,
        info.module);
    const std::string entrypoint(info.pName);
    std::hash<std::string> hasher;
    internal::hashCombine(hash, hasher(entrypoint));
    if (specialization)
        internal::hashCombine(hash, specialization->hash());
    return hash;
}
} // namespace magma
