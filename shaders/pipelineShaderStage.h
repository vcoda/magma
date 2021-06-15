/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2020 Victor Coda.

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
#include "specialization.h"

namespace magma
{
    class ShaderModule;
    class Specialization;

    /* Presence of a shader stage in a pipeline is indicated by including a valid
       shader stage info with module and an entry point from a shader module,
       where that entry point is valid for the specified stage. */

    class PipelineShaderStage : public VkPipelineShaderStageCreateInfo
    {
    public:
        explicit PipelineShaderStage(const VkShaderStageFlagBits stage,
            std::shared_ptr<ShaderModule> module,
            const char *const entrypoint,
            std::shared_ptr<Specialization> specialization = nullptr,
            VkPipelineShaderStageCreateFlags flags = 0) noexcept;
        PipelineShaderStage(const PipelineShaderStage&) noexcept;
        PipelineShaderStage& operator=(const PipelineShaderStage&) noexcept;
        virtual ~PipelineShaderStage();
        std::shared_ptr<ShaderModule> getShaderModule() noexcept { return shaderModule; }
        std::shared_ptr<const ShaderModule> getShaderModule() const noexcept { return shaderModule; }
        bool hasSpecialization() const noexcept { return specialization != nullptr; }
        std::size_t getHash() const noexcept;

    private:
        std::shared_ptr<ShaderModule> shaderModule;
        std::shared_ptr<Specialization> specialization;
    };
} // namespace magma
