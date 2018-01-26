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
#include <fstream>
#include "shaderModule.h"
#include "device.h"
#include "../shared.h"

namespace magma
{
ShaderModule::ShaderModule(std::shared_ptr<const Device> device, const uint32_t *bytecode, size_t bytecodeSize):
    NonDispatchable(VK_DEBUG_REPORT_OBJECT_TYPE_SHADER_MODULE_EXT, device)
{
    MAGMA_ASSERT(bytecodeSize % sizeof(uint32_t) == 0);
    VkShaderModuleCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.codeSize = bytecodeSize;
    info.pCode = bytecode;
    const VkResult create = vkCreateShaderModule(*device, &info, nullptr, &handle);
    MAGMA_THROW_FAILURE(create, "failed to create shader module");
}

ShaderModule::ShaderModule(std::shared_ptr<const Device> device, const std::vector<uint32_t>& bytecode):
    ShaderModule(device, bytecode.data(), bytecode.size() * sizeof(uint32_t))
{}    

ShaderModule::ShaderModule(std::shared_ptr<const Device> device, const std::string& filename):
    ShaderModule(device, loadSPIRVBytecode(filename))
{}

ShaderModule::~ShaderModule()
{
    vkDestroyShaderModule(*device, handle, nullptr);
}

std::vector<uint32_t> ShaderModule::loadSPIRVBytecode(const std::string& filename) const
{
    std::vector<uint32_t> bytecode;
    std::ifstream file(filename, std::ios::binary | std::ios::in | std::ios::ate);
    if (!file.is_open())
        MAGMA_THROW("failed to open file \"" + filename + "\"");
    const std::streamsize bytecodeSize = file.tellg();
    if (!bytecodeSize)
        MAGMA_THROW("file \"" + filename + "\" is empty");
    file.seekg(0, std::ios::beg);
    MAGMA_ASSERT(bytecodeSize % sizeof(uint32_t) == 0);
    bytecode.resize(static_cast<size_t>(bytecodeSize) / sizeof(uint32_t));
    file.read(reinterpret_cast<char *>(bytecode.data()), bytecodeSize);
    file.close();
    return bytecode;
}

ShaderStage::ShaderStage(const VkShaderStageFlagBits stage, std::shared_ptr<const ShaderModule> module, const char *const entrypoint,
    VkPipelineShaderStageCreateFlags flags /* 0 */, const VkSpecializationInfo *specializedInfo /* nullptr */) :
    module(module)
{
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = flags;
    info.stage = stage;
    info.module = *module;
    info.pName = helpers::copyString(entrypoint);
    info.pSpecializationInfo = specializedInfo;
}

ShaderStage::ShaderStage(const ShaderStage& other):
    info(other.info),
    module(other.module)
{
    info.pName = helpers::copyString(other.info.pName);
}

ShaderStage& ShaderStage::operator=(const ShaderStage& other)
{
    if (this != &other)
        info.pName = helpers::copyString(other.info.pName);
    return *this;
}

ShaderStage::~ShaderStage()
{
    delete[] info.pName;
}

VertexShaderStage::VertexShaderStage(std::shared_ptr<const ShaderModule> module, const char *const entrypoint,
    VkPipelineShaderStageCreateFlags flags /* 0 */, const VkSpecializationInfo *specializedInfo /* nullptr */) :
    ShaderStage(VK_SHADER_STAGE_VERTEX_BIT, module, entrypoint, flags, specializedInfo)
{}

GeometryShaderStage::GeometryShaderStage(std::shared_ptr<const ShaderModule> module, const char *const entrypoint,
    VkPipelineShaderStageCreateFlags flags /* 0 */, const VkSpecializationInfo *specializedInfo /* nullptr */) :
    ShaderStage(VK_SHADER_STAGE_GEOMETRY_BIT, module, entrypoint, flags, specializedInfo)
{}

FragmentShaderStage::FragmentShaderStage(std::shared_ptr<const ShaderModule> module, const char *const entrypoint,
    VkPipelineShaderStageCreateFlags flags /* 0 */, const VkSpecializationInfo *specializedInfo /* nullptr */) :
    ShaderStage(VK_SHADER_STAGE_FRAGMENT_BIT, module, entrypoint, flags, specializedInfo)
{}

ComputeShaderStage::ComputeShaderStage(std::shared_ptr<const ShaderModule> module, const char *const entrypoint,
    VkPipelineShaderStageCreateFlags flags /* 0 */, const VkSpecializationInfo *specializedInfo /* nullptr */) :
    ShaderStage(VK_SHADER_STAGE_COMPUTE_BIT, module, entrypoint, flags, specializedInfo)
{}
} // namespace magma
