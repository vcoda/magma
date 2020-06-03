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
#include "../shaders/shaderReflection.h"
#include "../allocator/allocator.h"
#include "../misc/exception.h"

namespace magma
{
ShaderModule::ShaderModule(std::shared_ptr<Device> device, const SpirvWord *bytecode, std::size_t bytecodeSize,
    std::size_t bytecodeHash /* 0 */, VkShaderModuleCreateFlags flags /* 0 */, bool reflect /* false */,
    std::shared_ptr<IAllocator> allocator /* nullptr */
#ifdef VK_EXT_validation_cache
    ,std::shared_ptr<ValidationCache> validationCache /* nullptr */
#endif
    ): NonDispatchable(VK_OBJECT_TYPE_SHADER_MODULE, std::move(device), std::move(allocator)),
       reflection(reflect ? std::make_shared<ShaderReflection>(bytecode, bytecodeSize) : nullptr)
{
#ifdef VK_EXT_validation_cache
    VkShaderModuleValidationCacheCreateInfoEXT cacheCreateInfo = {};
#endif
    VkShaderModuleCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
#ifdef VK_EXT_validation_cache
    if (validationCache)
    {
        cacheCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_VALIDATION_CACHE_CREATE_INFO_EXT;
        cacheCreateInfo.pNext = nullptr;
        cacheCreateInfo.validationCache = MAGMA_OPTIONAL_HANDLE(validationCache);
        info.pNext = &cacheCreateInfo;
    }
    else
        info.pNext = nullptr;
#else
    info.pNext = nullptr;
#endif // VK_EXT_validation_cache
    info.flags = flags;
    MAGMA_ASSERT(0 == bytecodeSize % sizeof(SpirvWord)); // A module is defined as a stream of words, not a stream of bytes
    info.codeSize = bytecodeSize;
    info.pCode = bytecode;
    const VkResult create = vkCreateShaderModule(MAGMA_HANDLE(device), &info, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
    MAGMA_THROW_FAILURE(create, "failed to create shader module");
    hash = core::hashArgs(
        info.sType,
        info.flags,
        info.codeSize);
#ifdef VK_EXT_validation_cache
    if (validationCache)
    {
        core::hashCombine(hash, core::hashArgs(
            cacheCreateInfo.sType,
            cacheCreateInfo.validationCache));
    }
#endif
    if (bytecodeHash)
        core::hashCombine(hash, bytecodeHash);
    else
    {   // Store bytecode for future hash computation
        this->bytecode.resize(info.codeSize); // codeSize is the size, in bytes, of the code pointed to by pCode
        memcpy(this->bytecode.data(), info.pCode, info.codeSize);
    }
}

ShaderModule::ShaderModule(std::shared_ptr<Device> device, const std::vector<SpirvWord>& bytecode,
    std::size_t bytecodeHash /* 0 */, VkShaderModuleCreateFlags flags /* 0 */, bool reflect /* false */,
    std::shared_ptr<IAllocator> allocator /* nullptr */
#ifdef VK_EXT_validation_cache
    ,std::shared_ptr<ValidationCache> validationCache /* nullptr */
#endif
    ): ShaderModule(std::move(device), bytecode.data(), bytecode.size() * sizeof(SpirvWord), bytecodeHash, flags, reflect,
        std::move(allocator)
#ifdef VK_EXT_validation_cache
        ,std::move(validationCache)
#endif
    )
{}

ShaderModule::~ShaderModule()
{
    vkDestroyShaderModule(MAGMA_HANDLE(device), handle, MAGMA_OPTIONAL_INSTANCE(allocator));
}

std::size_t ShaderModule::getHash() noexcept
{
    if (!bytecode.empty())
    {   // Compute hash on demand, may take time for large shaders
        const std::size_t bytecodeHash = core::hashVector(bytecode);
        core::hashCombine(hash, bytecodeHash);
        std::vector<char>().swap(bytecode);
    }
    return hash;
}
} // namespace magma
