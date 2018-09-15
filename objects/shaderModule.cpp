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
#include "shaderModule.h"
#include "device.h"
#include "../allocator/allocator.h"
#include "../helpers/hash.h"
#include "../misc/exception.h"

namespace magma
{
ShaderModule::ShaderModule(std::shared_ptr<Device> device, const uint32_t *bytecode, size_t bytecodeSize,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    NonDispatchable(VK_DEBUG_REPORT_OBJECT_TYPE_SHADER_MODULE_EXT, std::move(device), std::move(allocator))
{
    MAGMA_ASSERT(bytecodeSize % sizeof(uint32_t) == 0);
    VkShaderModuleCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.codeSize = bytecodeSize;
    info.pCode = bytecode;
    const VkResult create = vkCreateShaderModule(MAGMA_HANDLE(device), &info, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
    MAGMA_THROW_FAILURE(create, "failed to create shader module");
}

ShaderModule::ShaderModule(std::shared_ptr<Device> device, const std::vector<uint32_t>& bytecode,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    ShaderModule(device, bytecode.data(), bytecode.size() * sizeof(uint32_t), allocator)
{}

ShaderModule::~ShaderModule()
{
    vkDestroyShaderModule(MAGMA_HANDLE(device), handle, MAGMA_OPTIONAL_INSTANCE(allocator));
}

Specialization::Specialization(const Specialization& other)
{
    mapEntryCount = other.mapEntryCount;
    pMapEntries = helpers::copyArray(other.pMapEntries, mapEntryCount);
    dataSize = other.dataSize;
    try {
        pData = helpers::copyArray(reinterpret_cast<const char *>(other.pData), dataSize);
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
        pMapEntries = helpers::copyArray(other.pMapEntries, mapEntryCount);
        dataSize = other.dataSize;
        try {
            pData = helpers::copyArray(reinterpret_cast<const char *>(other.pData), dataSize);
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
        helpers::hashCombine(hash, helpers::hashVariadic(
            pMapEntries[i].constantID,
            pMapEntries[i].offset,
            pMapEntries[i].size));
    }
    helpers::hashCombine(hash, helpers::hashArray(
        reinterpret_cast<const uint8_t *>(pData), dataSize));
    return hash;
}

ShaderStage::ShaderStage(const VkShaderStageFlagBits stage, std::shared_ptr<const ShaderModule> module, const char *const entrypoint,
    std::shared_ptr<const Specialization> specialization, VkPipelineShaderStageCreateFlags flags) noexcept:
    module(std::move(module)),
    specialization(std::move(specialization))
{
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = flags;
    info.stage = stage;
    info.module = *this->module;
    info.pName = helpers::copyString(entrypoint);
    info.pSpecializationInfo = this->specialization.get();
}

ShaderStage::ShaderStage(const ShaderStage& other):
    info(other.info),
    module(other.module),
    specialization(other.specialization)
{
    info.pName = helpers::copyString(other.info.pName);
}

ShaderStage& ShaderStage::operator=(const ShaderStage& other)
{
    if (this != &other)
    {
        info = other.info;
        info.pName = helpers::copyString(other.info.pName);
        module = other.module;
        specialization = other.specialization;
    }
    return *this;
}

ShaderStage::~ShaderStage()
{
    delete[] info.pName;
}

size_t ShaderStage::hash() const noexcept
{
    size_t hash = helpers::hashVariadic(
        info.flags,
        info.stage,
        info.module);
    const std::string entrypoint(info.pName);
    std::hash<std::string> hasher;
    helpers::hashCombine(hash, hasher(entrypoint));
    if (specialization)
        helpers::hashCombine(hash, specialization->hash());
    return hash;
}

VertexShaderStage::VertexShaderStage(std::shared_ptr<const ShaderModule> module, const char *const entrypoint,
    std::shared_ptr<const Specialization> specialization /* nullptr */,
    VkPipelineShaderStageCreateFlags flags /* 0 */) noexcept:
    ShaderStage(VK_SHADER_STAGE_VERTEX_BIT, std::move(module), entrypoint, std::move(specialization), flags)
{}

TesselationControlShaderStage::TesselationControlShaderStage(std::shared_ptr<const ShaderModule> module, const char *const entrypoint,
    std::shared_ptr<const Specialization> specialization /* nullptr */,
    VkPipelineShaderStageCreateFlags flags /* 0 */) noexcept:
    ShaderStage(VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT, std::move(module), entrypoint, std::move(specialization), flags)
{}

TesselationEvaluationShaderStage::TesselationEvaluationShaderStage(std::shared_ptr<const ShaderModule> module, const char *const entrypoint,
    std::shared_ptr<const Specialization> specialization /* nullptr */,
    VkPipelineShaderStageCreateFlags flags /* 0 */) noexcept:
    ShaderStage(VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT, std::move(module), entrypoint, std::move(specialization), flags)
{}

GeometryShaderStage::GeometryShaderStage(std::shared_ptr<const ShaderModule> module, const char *const entrypoint,
    std::shared_ptr<const Specialization> specialization /* nullptr */,
    VkPipelineShaderStageCreateFlags flags /* 0 */) noexcept:
    ShaderStage(VK_SHADER_STAGE_GEOMETRY_BIT, std::move(module), entrypoint, std::move(specialization), flags)
{}

FragmentShaderStage::FragmentShaderStage(std::shared_ptr<const ShaderModule> module, const char *const entrypoint,
    std::shared_ptr<const Specialization> specialization /* nullptr */,
    VkPipelineShaderStageCreateFlags flags /* 0 */) noexcept:
    ShaderStage(VK_SHADER_STAGE_FRAGMENT_BIT, std::move(module), entrypoint, std::move(specialization), flags)
{}

ComputeShaderStage::ComputeShaderStage(std::shared_ptr<const ShaderModule> module, const char *const entrypoint,
    std::shared_ptr<const Specialization> specialization /* nullptr */,
    VkPipelineShaderStageCreateFlags flags /* 0 */) noexcept:
    ShaderStage(VK_SHADER_STAGE_COMPUTE_BIT, std::move(module), entrypoint, std::move(specialization), flags)
{}
} // namespace magma
