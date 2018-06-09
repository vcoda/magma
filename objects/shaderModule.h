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
#pragma once
#include <vector>
#include "handle.h"
#include "../helpers/copy.h"

namespace magma
{
    class ShaderModule : public NonDispatchable<VkShaderModule>
    {
    public:
        ShaderModule(std::shared_ptr<const Device> device,
            const uint32_t *bytecode,
            size_t bytecodeSize,
            std::shared_ptr<IAllocator> allocator = nullptr);
        ShaderModule(std::shared_ptr<const Device> device,
            const std::vector<uint32_t>& bytecode,
            std::shared_ptr<IAllocator> allocator = nullptr);
        ~ShaderModule();
    };

    struct SpecializationEntry : VkSpecializationMapEntry
    {
        template<typename Block, typename Type>
        SpecializationEntry(uint32_t index, Type Block::*member)
        {                    
            constantID = index;
            const ptrdiff_t diff = reinterpret_cast<ptrdiff_t>(&(((Block*)0)->*member));
            offset = static_cast<uint32_t>(diff);
            size = sizeof(Type);
        }
    };

    class Specialization final : public VkSpecializationInfo
    {
    public:
        template<typename Block>
        Specialization(const Block& data, 
            const std::initializer_list<SpecializationEntry>& entryMap)
        {
            mapEntryCount = static_cast<uint32_t>(entryMap.size());
            pMapEntries = magma::helpers::copy(new VkSpecializationMapEntry[mapEntryCount], entryMap);
            dataSize = sizeof(Block);
            pData = magma::helpers::copy(new Block[1], &data);
        }
        Specialization(const Specialization&);
        Specialization& operator=(const Specialization&);
        ~Specialization();
    };

    class ShaderStage
    {
    protected:
        ShaderStage(const VkShaderStageFlagBits stage,
            std::shared_ptr<const ShaderModule> module,
            const char *const entrypoint,
            std::shared_ptr<const Specialization> specialization,
            VkPipelineShaderStageCreateFlags flags);

    public:
        ShaderStage(const ShaderStage&);
        ShaderStage& operator=(const ShaderStage&);
        ~ShaderStage();
        VkShaderStageFlagBits getStage() const { return info.stage; }
        operator const VkPipelineShaderStageCreateInfo&() const { return info; }

    private:
        VkPipelineShaderStageCreateInfo info;
        std::shared_ptr<const ShaderModule> module;
        std::shared_ptr<const Specialization> specialization;
    };

    class VertexShaderStage : public ShaderStage
    {
    public:
        VertexShaderStage(std::shared_ptr<const ShaderModule> module,
            const char *const entrypoint,
            std::shared_ptr<const Specialization> specialization = nullptr,
            VkPipelineShaderStageCreateFlags flags = 0);
    };

    class GeometryShaderStage : public ShaderStage
    {
    public:
        GeometryShaderStage(std::shared_ptr<const ShaderModule> module,
            const char *const entrypoint,
            std::shared_ptr<const Specialization> specialization = nullptr,
            VkPipelineShaderStageCreateFlags flags = 0);
    };

    class FragmentShaderStage : public ShaderStage
    {
    public:
        FragmentShaderStage(std::shared_ptr<const ShaderModule> module,
            const char *const entrypoint,
            std::shared_ptr<const Specialization> specialization = nullptr,
            VkPipelineShaderStageCreateFlags flags = 0);
    };

    class ComputeShaderStage : public ShaderStage
    {
    public:
        ComputeShaderStage(std::shared_ptr<const ShaderModule> module,
            const char *const entrypoint,
            std::shared_ptr<const Specialization> specialization = nullptr,
            VkPipelineShaderStageCreateFlags flags = 0);
    };
} // namespace magma
