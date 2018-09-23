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
#include <memory>
#include "../vulkan.h"
#include "../helpers/copy.h"

namespace magma
{
    class ShaderModule;

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

    class PipelineShaderStage
    {
    protected:
        explicit PipelineShaderStage(const VkShaderStageFlagBits stage,
            std::shared_ptr<const ShaderModule> module,
            const char *const entrypoint,
            std::shared_ptr<const Specialization> specialization,
            VkPipelineShaderStageCreateFlags flags) noexcept;

    public:
        PipelineShaderStage(const PipelineShaderStage&);
        PipelineShaderStage& operator=(const PipelineShaderStage&);
        virtual ~PipelineShaderStage();
        VkShaderStageFlagBits getStage() const noexcept { return info.stage; }
        size_t hash() const noexcept;
        operator const VkPipelineShaderStageCreateInfo&() const noexcept { return info; }

    private:
        VkPipelineShaderStageCreateInfo info;
        std::shared_ptr<const ShaderModule> module;
        std::shared_ptr<const Specialization> specialization;
    };

    class VertexShaderStage : public PipelineShaderStage
    {
    public:
        explicit VertexShaderStage(std::shared_ptr<const ShaderModule> module,
            const char *const entrypoint,
            std::shared_ptr<const Specialization> specialization = nullptr,
            VkPipelineShaderStageCreateFlags flags = 0) noexcept:
        PipelineShaderStage(VK_SHADER_STAGE_VERTEX_BIT,
            std::move(module), entrypoint, std::move(specialization), flags) {}
    };

    class TesselationControlShaderStage : public PipelineShaderStage
    {
    public:
        explicit TesselationControlShaderStage(std::shared_ptr<const ShaderModule> module,
            const char *const entrypoint,
            std::shared_ptr<const Specialization> specialization = nullptr,
            VkPipelineShaderStageCreateFlags flags = 0) noexcept:
        PipelineShaderStage(VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT,
            std::move(module), entrypoint, std::move(specialization), flags) {}
    };

    class TesselationEvaluationShaderStage : public PipelineShaderStage
    {
    public:
        explicit TesselationEvaluationShaderStage(std::shared_ptr<const ShaderModule> module,
            const char *const entrypoint,
            std::shared_ptr<const Specialization> specialization = nullptr,
            VkPipelineShaderStageCreateFlags flags = 0) noexcept:
        PipelineShaderStage(VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT,
            std::move(module), entrypoint, std::move(specialization), flags) {}
    };

    class GeometryShaderStage : public PipelineShaderStage
    {
    public:
        explicit GeometryShaderStage(std::shared_ptr<const ShaderModule> module,
            const char *const entrypoint,
            std::shared_ptr<const Specialization> specialization = nullptr,
            VkPipelineShaderStageCreateFlags flags = 0) noexcept:
        PipelineShaderStage(VK_SHADER_STAGE_GEOMETRY_BIT, std::move(module),
            entrypoint, std::move(specialization), flags) {}
    };

    class FragmentShaderStage : public PipelineShaderStage
    {
    public:
        explicit FragmentShaderStage(std::shared_ptr<const ShaderModule> module,
            const char *const entrypoint,
            std::shared_ptr<const Specialization> specialization = nullptr,
            VkPipelineShaderStageCreateFlags flags = 0) noexcept:
        PipelineShaderStage(VK_SHADER_STAGE_FRAGMENT_BIT, std::move(module),
            entrypoint, std::move(specialization), flags) {}
    };

    class ComputeShaderStage : public PipelineShaderStage
    {
    public:
        explicit ComputeShaderStage(std::shared_ptr<const ShaderModule> module,
            const char *const entrypoint,
            std::shared_ptr<const Specialization> specialization = nullptr,
            VkPipelineShaderStageCreateFlags flags = 0) noexcept:
        PipelineShaderStage(VK_SHADER_STAGE_COMPUTE_BIT, std::move(module),
            entrypoint, std::move(specialization), flags) {}
    };
} // namespace magma
