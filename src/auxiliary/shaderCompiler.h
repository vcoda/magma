/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2025 Victor Coda.

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

typedef struct shaderc_compiler* shaderc_compiler_t;

namespace magma
{
    class Device;
    class ShaderModule;

    namespace aux
    {
        enum class IncludePath : uint8_t
        {
            Relative, // #include "source"
            Standard  // #include <source>
        };

        enum class OptimizationLevel : uint8_t
        {
            Disabled, Compact, Performance
        };

        /* An abstract interface for mapping an #include request
           to a result source string. */

        class IShaderInclude : public IClass
        {
        public:
            virtual void *resolve(IncludePath pathType,
                const char *requestedSource,
                const char *requestingSource,
                std::size_t includeDepth,
                std::size_t& dataSize) noexcept = 0;
            virtual void release(void *data) noexcept = 0;
        };

        /*  Run-time shader compiler built on top of github.com/google/shaderc.

            glslc wraps around core functionality in glslang and SPIRV-Tools.
            glslc and its library aims to to provide:
             * a command line compiler with GCC- and Clang-like usage, for better
               integration with build systems.
             * an API where functionality can be added without breaking existing clients.
             * an API supporting standard concurrency patterns across multiple
               operating systems.
             * increased functionality such as file #include support. */

        class ShaderCompiler : public IClass
        {
        public:
            explicit ShaderCompiler(std::shared_ptr<Device> device,
                std::unique_ptr<IShaderInclude> includeHandler = nullptr);
            ~ShaderCompiler();
            void setOptimizationLevel(OptimizationLevel optimizationLevel_) noexcept { optimizationLevel = optimizationLevel_; }
            OptimizationLevel getOptimizationLevel() const noexcept { return optimizationLevel; }
            void setGenerateDebugInfo(bool generateDebugInfo_) noexcept { generateDebugInfo = generateDebugInfo_; }
            bool getGenerateDebugInfo() const noexcept { return generateDebugInfo; }
            void setSuppressWarnings(bool suppressWarnings_) noexcept { suppressWarnings_ = suppressWarnings; }
            bool getSuppressWarnings() const noexcept { return suppressWarnings; }
            void setWarningsAsErrors(bool warningsAsErrors_) noexcept { warningsAsErrors = warningsAsErrors_; }
            bool getWarningsAsErrors() const noexcept { return warningsAsErrors; }
            std::shared_ptr<ShaderModule> compileShader(std::string_view source,
                std::string_view entrypoint,
                VkShaderStageFlagBits shaderStage = (VkShaderStageFlagBits)0,
                const std::unordered_map<std::string_view, std::string_view>& macroDefinitions = {},
                std::string_view srcFileName = {});

        private:
            std::shared_ptr<Device> device;
            std::unique_ptr<IShaderInclude> includeHandler;
            shaderc_compiler_t compiler;
            OptimizationLevel optimizationLevel;
            bool generateDebugInfo;
            bool suppressWarnings;
            bool warningsAsErrors;
        };
    } // namespace aux
} // namespace magma
