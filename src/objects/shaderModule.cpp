/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2023 Victor Coda.

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
#include "../exceptions/errorResult.h"

namespace magma
{
ShaderModule::ShaderModule(std::shared_ptr<Device> device, const SpirvWord *bytecode, std::size_t bytecodeSize,
    hash_t bytecodeHash /* 0 */,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    bool reflect /* false */,
    VkShaderModuleCreateFlags flags /* 0 */,
#ifdef VK_EXT_validation_cache
    std::shared_ptr<ValidationCache> validationCache /* nullptr */,
#endif
    const StructureChain& extendedInfo /* default */):
    NonDispatchable(VK_OBJECT_TYPE_SHADER_MODULE, std::move(device), std::move(allocator)),
    hash(0),
    bytecodeHash(bytecodeHash)
{   // A module is defined as a stream of words, not a stream of bytes
    MAGMA_ASSERT(bytecodeSize % sizeof(SpirvWord) == 0);
    VkShaderModuleCreateInfo shaderModuleInfo;
    shaderModuleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderModuleInfo.pNext = extendedInfo.headNode();
    shaderModuleInfo.flags = flags;
    shaderModuleInfo.codeSize = bytecodeSize;
    shaderModuleInfo.pCode = bytecode;
#ifdef VK_EXT_validation_cache
    VkShaderModuleValidationCacheCreateInfoEXT shaderModuleValidationCacheInfo = {};
    if (validationCache)
    {
        shaderModuleInfo.pNext = &shaderModuleValidationCacheInfo;
        shaderModuleValidationCacheInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_VALIDATION_CACHE_CREATE_INFO_EXT;
        shaderModuleValidationCacheInfo.pNext = extendedInfo.headNode();
        shaderModuleValidationCacheInfo.validationCache = MAGMA_OPTIONAL_HANDLE(validationCache);
    }
#endif // VK_EXT_validation_cache
    const VkResult result = vkCreateShaderModule(MAGMA_HANDLE(device), &shaderModuleInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to create shader module");
    if (reflect)
        reflection = std::make_shared<ShaderReflection>(shaderModuleInfo.pCode, shaderModuleInfo.codeSize);
    hash = core::hashArgs(
        shaderModuleInfo.sType,
        shaderModuleInfo.flags,
        shaderModuleInfo.codeSize,
        extendedInfo.hash());
#ifdef VK_EXT_validation_cache
    if (validationCache)
    {
        hash = core::hashCombine(hash, core::hashArgs(
            shaderModuleValidationCacheInfo.sType,
            shaderModuleValidationCacheInfo.validationCache));
    }
#endif // VK_EXT_validation_cache
    if (!bytecodeHash && !reflect)
    {   // Store bytecode for on-demand hash computation
        this->bytecode.resize(bytecodeSize / sizeof(SpirvWord));
        memcpy(this->bytecode.data(), bytecode, bytecodeSize);
    }
}

ShaderModule::ShaderModule(std::shared_ptr<Device> device, const std::vector<SpirvWord>& bytecode,
    hash_t bytecodeHash /* 0 */,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    bool reflect /* false */,
    VkShaderModuleCreateFlags flags /* 0 */,
#ifdef VK_EXT_validation_cache
    std::shared_ptr<ValidationCache> validationCache /* nullptr */,
#endif
    const StructureChain& extendedInfo /* default */):
    ShaderModule(std::move(device), bytecode.data(), bytecode.size() * sizeof(SpirvWord), bytecodeHash, std::move(allocator), reflect, flags,
    #ifdef VK_EXT_validation_cache
        std::move(validationCache),
    #endif
        extendedInfo)
{}

ShaderModule::~ShaderModule()
{
    vkDestroyShaderModule(MAGMA_HANDLE(device), handle, MAGMA_OPTIONAL_INSTANCE(hostAllocator));
}

hash_t ShaderModule::getHash() const noexcept
{
    return core::hashCombine(hash, getBytecodeHash());
}

hash_t ShaderModule::getBytecodeHash() const noexcept
{
    if (!bytecodeHash)
    {   // Compute on demand
        if (!bytecode.empty())
        {
            bytecodeHash = core::hashArray(bytecode.data(), bytecode.size());
            std::vector<SpirvWord>().swap(bytecode);
        }
        else if (reflection)
        {
            auto wordCount = reflection->getCodeSize() / sizeof(SpirvWord);
            bytecodeHash = core::hashArray(reflection->getCode(), wordCount);
        }
    }
    return bytecodeHash;
}
} // namespace magma
