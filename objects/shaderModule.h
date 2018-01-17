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

namespace magma
{
    class Device;

    class ShaderModule : public NonDispatchable<VkShaderModule>
    {
    public:
        ShaderModule(std::shared_ptr<const Device> device,
            const uint32_t *bytecode,
            size_t bytecodeSize);
        ShaderModule(std::shared_ptr<const Device> device,
            const std::vector<uint32_t>& bytecode);
        ShaderModule(std::shared_ptr<const Device>,
            const std::string& filename);
        ~ShaderModule();

    private:
        std::vector<uint32_t> loadSPIRVBytecode(const std::string& filename) const;
    };

    class ShaderStage
    {
    public:
        ShaderStage(const VkShaderStageFlagBits stage,
            std::shared_ptr<const ShaderModule> module,
            const char *const entrypoint,
            VkPipelineShaderStageCreateFlags flags = 0,
            const VkSpecializationInfo *specializedInfo = nullptr);
        ShaderStage(const ShaderStage&);
        ~ShaderStage();
        operator const VkPipelineShaderStageCreateInfo&() const { return info; }

    private:
        VkPipelineShaderStageCreateInfo info;
        std::shared_ptr<const ShaderModule> module;
    };

    class VertexShaderStage : public ShaderStage
    {
    public:
        VertexShaderStage(std::shared_ptr<const ShaderModule> module,
            const char *const entrypoint,
            VkPipelineShaderStageCreateFlags flags = 0,
            const VkSpecializationInfo *specializedInfo = nullptr);
    };

    class GeometryShaderStage : public ShaderStage
    {
    public:
        GeometryShaderStage(std::shared_ptr<const ShaderModule> module,
            const char *const entrypoint,
            VkPipelineShaderStageCreateFlags flags = 0,
            const VkSpecializationInfo *specializedInfo = nullptr);
    };

    class FragmentShaderStage : public ShaderStage
    {
    public:
        FragmentShaderStage(std::shared_ptr<const ShaderModule> module,
            const char *const entrypoint,
            VkPipelineShaderStageCreateFlags flags = 0,
            const VkSpecializationInfo *specializedInfo = nullptr);
    };

    class ComputeShaderStage : public ShaderStage
    {
    public:
        ComputeShaderStage(std::shared_ptr<const ShaderModule> module,
            const char *const entrypoint,
            VkPipelineShaderStageCreateFlags flags = 0,
            const VkSpecializationInfo *specializedInfo = nullptr);
    };
} // namespace magma
