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
    /* Shader modules contain shader code and one or more entry points.
       Shaders are selected from a shader module by specifying an entry point.
       The stages of a pipeline can use shaders that come from different modules.
       The shader code defining a shader module must be in the SPIR-V format. */

    class ShaderModule : public NonDispatchable<VkShaderModule>
    {
    public:
        explicit ShaderModule(std::shared_ptr<Device> device,
            const uint32_t *bytecode,
            size_t bytecodeSize,
            std::shared_ptr<IAllocator> allocator = nullptr);
        explicit ShaderModule(std::shared_ptr<Device> device,
            const std::vector<uint32_t>& bytecode,
            std::shared_ptr<IAllocator> allocator = nullptr);
        ~ShaderModule();
    };

    struct SpecializationEntry : VkSpecializationMapEntry
    {
        SpecializationEntry() noexcept {}
        template<typename Block, typename Type>
        explicit SpecializationEntry(uint32_t index, Type Block::*member) noexcept
        {
            constantID = index;
            const ptrdiff_t diff = reinterpret_cast<ptrdiff_t>(&(((Block*)0)->*member));
            offset = static_cast<uint32_t>(diff);
            size = sizeof(Type);
        }
    };

    /* Specialization constants are a mechanism whereby constants in a SPIR-V module
       can have their constant value specified at the time the pipeline is created.
       This allows a SPIR-V module to have constants that can be modified while
       executing an application that uses the Vulkan API. */

    class Specialization final : public VkSpecializationInfo
    {
    public:
        template<typename Block>
        explicit Specialization(const Block& data,
            const std::initializer_list<SpecializationEntry>& entryMap)
        {
            mapEntryCount = static_cast<uint32_t>(entryMap.size());
            pMapEntries = helpers::copyInitializer(entryMap);
            dataSize = sizeof(Block);
            pData = helpers::copyArray(reinterpret_cast<const char *>(&data), dataSize);
        }
        Specialization(const Specialization&);
        Specialization& operator=(const Specialization&);
        ~Specialization();
        size_t hash() const noexcept;
    };

    class ShaderStage
    {
    protected:
        explicit ShaderStage(const VkShaderStageFlagBits stage,
            std::shared_ptr<const ShaderModule> module,
            const char *const entrypoint,
            std::shared_ptr<const Specialization> specialization,
            VkPipelineShaderStageCreateFlags flags) noexcept;

    public:
        ShaderStage(const ShaderStage&);
        ShaderStage& operator=(const ShaderStage&);
        virtual ~ShaderStage();
        VkShaderStageFlagBits getStage() const noexcept { return info.stage; }
        size_t hash() const noexcept;
        operator const VkPipelineShaderStageCreateInfo&() const noexcept { return info; }

    private:
        VkPipelineShaderStageCreateInfo info;
        std::shared_ptr<const ShaderModule> module;
        std::shared_ptr<const Specialization> specialization;
    };

    class VertexShaderStage : public ShaderStage
    {
    public:
        explicit VertexShaderStage(std::shared_ptr<const ShaderModule> module,
            const char *const entrypoint,
            std::shared_ptr<const Specialization> specialization = nullptr,
            VkPipelineShaderStageCreateFlags flags = 0) noexcept;
    };

    class TesselationControlShaderStage : public ShaderStage
    {
    public:
        explicit TesselationControlShaderStage(std::shared_ptr<const ShaderModule> module,
            const char *const entrypoint,
            std::shared_ptr<const Specialization> specialization = nullptr,
            VkPipelineShaderStageCreateFlags flags = 0) noexcept;
    };

    class TesselationEvaluationShaderStage : public ShaderStage
    {
    public:
        explicit TesselationEvaluationShaderStage(std::shared_ptr<const ShaderModule> module,
            const char *const entrypoint,
            std::shared_ptr<const Specialization> specialization = nullptr,
            VkPipelineShaderStageCreateFlags flags = 0) noexcept;
    };

    class GeometryShaderStage : public ShaderStage
    {
    public:
        explicit GeometryShaderStage(std::shared_ptr<const ShaderModule> module,
            const char *const entrypoint,
            std::shared_ptr<const Specialization> specialization = nullptr,
            VkPipelineShaderStageCreateFlags flags = 0) noexcept;
    };

    class FragmentShaderStage : public ShaderStage
    {
    public:
        explicit FragmentShaderStage(std::shared_ptr<const ShaderModule> module,
            const char *const entrypoint,
            std::shared_ptr<const Specialization> specialization = nullptr,
            VkPipelineShaderStageCreateFlags flags = 0) noexcept;
    };

    class ComputeShaderStage : public ShaderStage
    {
    public:
        explicit ComputeShaderStage(std::shared_ptr<const ShaderModule> module,
            const char *const entrypoint,
            std::shared_ptr<const Specialization> specialization = nullptr,
            VkPipelineShaderStageCreateFlags flags = 0) noexcept;
    };
} // namespace magma
