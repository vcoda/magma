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
#ifdef _WIN32
#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include <shaderc/shaderc.h>
#include "../misc/exception.h"
#include "../internal/nonCopyable.h"

namespace magma
{
    class Device;
    class ShaderModule;

    namespace aux
    {
        /* An abstract interface for mapping an #include request to an include result. */

        class IShaderInclude : public internal::NonCopyable
        {
        public:
            virtual void *resolve(shaderc_include_type includeType,
                const char *requestedSource,
                const char *requestingSource,
                size_t includeDepth,
                size_t& dataSize) noexcept = 0;
            virtual void release(void *data) noexcept = 0;
        };

        /* Auxiliary wrapper around https://github.com/google/shaderc
           to compile GLSL shaders in run-time. Can be useful in case
           if shader code is generated dynamically. */

        class ShaderCompiler : public internal::NonCopyable
        {
        public:
            ShaderCompiler(std::shared_ptr<Device> device,
                std::shared_ptr<IShaderInclude> handler);
            ~ShaderCompiler();
            void setOptimizationLevel(shaderc_optimization_level level) noexcept;
            void setGenerateDebugInfo(bool generate) noexcept;
            void setSuppressWarnings(bool suppress) noexcept;
            void setWarningsAsErrors(bool errors) noexcept;
            std::shared_ptr<ShaderModule> compileShader(const std::string& source,
                const char *entrypoint,
                shaderc_shader_kind shaderKind = shaderc_glsl_infer_from_source,
                const std::unordered_map<std::string, std::string>& macroDefinitions = {},
                const std::string& srcFileName = "");

        private:
            std::shared_ptr<Device> device;
            std::shared_ptr<IShaderInclude> includeHandler;
            shaderc_compiler_t compiler;
            shaderc_optimization_level optimizationLevel = shaderc_optimization_level_performance;
            bool generateDebugInfo = false;
            bool suppressWarnings = false;
            bool warningsAsErrors = false;
        };

        class CompileException : public Exception
        {
        public:
            CompileException(shaderc_compilation_result_t result,
                const char *file, int line);
            shaderc_compilation_status getStatus() const noexcept { return status; }
            size_t numWarnings() const noexcept { return warnings; }
            size_t numErrors() const noexcept { return errors; }

        private:
            shaderc_compilation_status status;
            size_t warnings;
            size_t errors;
        };
    } // namespace aux
} // namespace magma
#endif // _WIN32
