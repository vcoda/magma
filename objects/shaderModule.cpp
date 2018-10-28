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
#include "../utilities/hash.h"
#include "../misc/exception.h"
#include "../shared.h"

namespace magma
{
ShaderModule::ShaderModule(std::shared_ptr<Device> device, const uint32_t *bytecode, size_t bytecodeSize,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    NonDispatchable(VK_OBJECT_TYPE_SHADER_MODULE, std::move(device), std::move(allocator))
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
} // namespace magma
