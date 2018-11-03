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
#include <string>
#include <unordered_map>
#include <memory>
#include <shaderc/shaderc.h>
#include "../nonCopyable.h"

namespace shaderc
{
    class Compiler;
}

namespace magma
{
    class Device;
    class ShaderModule;

    namespace aux
    {
        /* An abstract interface for mapping an #include request to an include result. */

        class IShaderIncludeHandler : public sys::NonCopyable
        {
        public:
            virtual shaderc_include_result *resolve(const char *requestedSource,
                shaderc_include_type includeType,
                const char* requestingSource,
                size_t includeDepth) = 0;
            virtual void release(shaderc_include_result *result) = 0;
        };

        /* Auxiliary wrapper around https://github.com/google/shaderc
           to compile GLSL shaders in run-time. Can be useful in case
           if shader code is generated dynamically. */

        class ShaderCompiler : public sys::NonCopyable
        {
        public:
            ShaderCompiler(std::shared_ptr<Device> device);
            ~ShaderCompiler();
            void setIncludeHandler(std::shared_ptr<IShaderIncludeHandler> handler) noexcept
                { includeHandler = handler; }
            void setOptimizationLevel(shaderc_optimization_level level) noexcept
                { optimizationLevel = level; }
            void setGenerateDebugInfo(bool generate) noexcept
                { generateDebugInfo = generate; }
            void setWarningsAsErrors(bool asErrors) noexcept
                { warningsAsErrors = asErrors; }
            std::shared_ptr<ShaderModule> compileShader(const std::string& source,
                const char *entrypoint,
                shaderc_shader_kind shaderKind = shaderc_glsl_infer_from_source,
                const std::unordered_map<std::string, std::string>& macroDefinitions = {});

        private:
            std::shared_ptr<Device> device;
            std::shared_ptr<IShaderIncludeHandler> includeHandler;
            shaderc_compiler_t compiler;
            shaderc_optimization_level optimizationLevel = shaderc_optimization_level_performance;
            bool generateDebugInfo = false;
            bool warningsAsErrors = false;
        };
    } // namespace aux
} // namespace magma
