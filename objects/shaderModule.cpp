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
#include "shaderModule.h"
#include "device.h"
#include "validationCache.h"
#include "../allocator/allocator.h"
#include "../misc/exception.h"

namespace magma
{
ShaderModule::ShaderModule(std::shared_ptr<Device> device, const SpirvWord *bytecode, std::size_t bytecodeSize,
    VkShaderModuleCreateFlags flags /* 0 */,
    std::shared_ptr<ValidationCache> validationCache /* nullptr */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    NonDispatchable(VK_OBJECT_TYPE_SHADER_MODULE, std::move(device), std::move(allocator)),
    hash(detail::hashArray(bytecode, bytecodeSize / sizeof(SpirvWord)))
{
    VkShaderModuleCreateInfo info;
    VkShaderModuleValidationCacheCreateInfoEXT cacheCreateInfo;
    info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    if (!validationCache)
        info.pNext = nullptr;
    else
    {
        cacheCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_VALIDATION_CACHE_CREATE_INFO_EXT;
        cacheCreateInfo.pNext = nullptr;
        cacheCreateInfo.validationCache = MAGMA_OPTIONAL_HANDLE(validationCache);
        info.pNext = &cacheCreateInfo;
    }
    if (flags != 0)
        detail::hashCombine(hash, detail::hash(flags));
    info.flags = flags;
    MAGMA_ASSERT(0 == bytecodeSize % sizeof(SpirvWord)); // A module is defined as a stream of words, not a stream of bytes
    info.codeSize = bytecodeSize;
    info.pCode = bytecode;
    const VkResult create = vkCreateShaderModule(MAGMA_HANDLE(device), &info, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
    MAGMA_THROW_FAILURE(create, "failed to create shader module");
}

ShaderModule::ShaderModule(std::shared_ptr<Device> device, const std::vector<SpirvWord>& bytecode,
    VkShaderModuleCreateFlags flags /* 0 */,
    std::shared_ptr<ValidationCache> validationCache /* nullptr */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    ShaderModule(std::move(device), bytecode.data(), bytecode.size() * sizeof(SpirvWord), flags,
        std::move(validationCache), std::move(allocator))
{}

ShaderModule::~ShaderModule()
{
    vkDestroyShaderModule(MAGMA_HANDLE(device), handle, MAGMA_OPTIONAL_INSTANCE(allocator));
}
} // namespace magma
