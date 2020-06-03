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
#include "../core/noncopyable.h"
#include "../third-party/SPIRV-Reflect/spirv_reflect.h"

namespace magma
{
    /* SPIRV-Reflect is a lightweight library that provides a C/C++ reflection API
       for SPIR-V shader bytecode in Vulkan applications.
       https://github.com/chaoticbob/SPIRV-Reflect */

    class ShaderReflection : public core::NonCopyable
    {
    public:
        ShaderReflection(std::size_t size, const void *bytecode);
        ~ShaderReflection() noexcept;
        uint32_t entryPointCount() const noexcept { return module.entry_point_count; }
        const char *entryPointName(uint32_t index) const noexcept { return module.entry_points[index].name; }
        VkShaderStageFlagBits shaderStage() const noexcept { return static_cast<VkShaderStageFlagBits>(module.shader_stage); };
        SpvSourceLanguage sourceLanguage() const noexcept { return module.source_language; }
        uint32_t sourceLanguageVersion() const noexcept { return module.source_language_version; }

    private:
        SpvReflectShaderModule module;
    };
} // namespace magma
