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
#include "specialization.h"

namespace magma
{
    class ShaderModule;
    class Specialization;

    class PipelineShaderStage : public VkPipelineShaderStageCreateInfo
    {
    protected:
        explicit PipelineShaderStage(const VkShaderStageFlagBits stage,
            std::shared_ptr<ShaderModule> module,
            const char *const entrypoint,
            std::shared_ptr<Specialization> specialization = nullptr,
            VkPipelineShaderStageCreateFlags flags = 0) noexcept;

    public:
        PipelineShaderStage(const PipelineShaderStage&) noexcept;
        PipelineShaderStage& operator=(const PipelineShaderStage&) noexcept;
        virtual ~PipelineShaderStage();
        std::shared_ptr<ShaderModule> getShaderModule() noexcept { return shaderModule; }
        std::shared_ptr<const ShaderModule> getShaderModule() const noexcept { return shaderModule; }
        std::size_t hash() const noexcept;

    private:
        std::shared_ptr<ShaderModule> shaderModule;
        std::shared_ptr<Specialization> specialization;
    };
} // namespace magma
