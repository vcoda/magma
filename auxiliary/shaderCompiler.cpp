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
#include "shaderCompiler.h"
#include "../objects/shaderModule.h"
#include "../misc/exception.h"
#include "../shared.h"

namespace magma
{
namespace aux
{
ShaderCompiler::ShaderCompiler(std::shared_ptr<Device> device, std::shared_ptr<IShaderInclude> handler):
    device(device),
    includeHandler(handler)
{
    compiler = shaderc_compiler_initialize();
}

ShaderCompiler::~ShaderCompiler()
{
    shaderc_compiler_release(compiler);
}

void ShaderCompiler::setOptimizationLevel(shaderc_optimization_level level) noexcept
{
    optimizationLevel = level;
}

void ShaderCompiler::setGenerateDebugInfo(bool generate) noexcept
{
    generateDebugInfo = generate;
}

void ShaderCompiler::setSuppressWarnings(bool suppress) noexcept
{
    suppressWarnings = suppress;
}

void ShaderCompiler::setWarningsAsErrors(bool errors) noexcept
{
    warningsAsErrors = errors;
}

std::shared_ptr<ShaderModule> ShaderCompiler::compileShader(const std::string& source, const char *entrypoint,
    shaderc_shader_kind shaderKind /* shaderc_glsl_infer_from_source */,
    const std::unordered_map<std::string, std::string>& macroDefinitions /* {} */,
    const std::string& srcFileName  /* "" */)
{
    MAGMA_ASSERT(source.length() > 0);
    MAGMA_ASSERT(strlen(entrypoint) > 0);
    shaderc_compile_options_t options = shaderc_compile_options_initialize();
    for (const auto& macro : macroDefinitions)
    {   // Add preprocessor definitions
        shaderc_compile_options_add_macro_definition(options,
            macro.first.c_str(), macro.first.length(),
            macro.second.c_str(), macro.second.length());
    }
    // Define compiler behavior
    shaderc_compile_options_set_optimization_level(options, optimizationLevel);
    if (generateDebugInfo)
        shaderc_compile_options_set_generate_debug_info(options);
    if (suppressWarnings)
        shaderc_compile_options_set_suppress_warnings(options);
    if (warningsAsErrors)
        shaderc_compile_options_set_warnings_as_errors(options);
    if (includeHandler)
    {   // Provide shader include callbacks
        shaderc_compile_options_set_include_callbacks(options,
            [](void *userData, const char *requestedSource, int type, const char *requestingSource, size_t includeDepth)
            {
                shaderc_include_result *result = new(std::nothrow)shaderc_include_result;
                if (result)
                {
                    IShaderInclude *includeHandler = (IShaderInclude *)userData;
                    result->source_name = requestedSource;
                    result->source_name_length = strlen(requestedSource);
                    result->content_length = 0;
                    result->content = (const char *)includeHandler->resolve(
                        static_cast<shaderc_include_type>(type), requestedSource, requestingSource,
                        includeDepth, result->content_length);
                    result->user_data = nullptr;
                }
                return result;
            },
            [](void* userData, shaderc_include_result *result)
            {
                IShaderInclude *includeHandler = (IShaderInclude *)userData;
                includeHandler->release((void *)result->content);
                delete result;
            },
            includeHandler.get());
    }
    // Compile GLSL to SPIR-V
    const shaderc_compilation_result_t result = shaderc_compile_into_spv(compiler,
        source.c_str(), source.size(), shaderKind, srcFileName.c_str(), entrypoint, options);
    shaderc_compile_options_release(options);
    const shaderc_compilation_status status = shaderc_result_get_compilation_status(result);
    if (status != shaderc_compilation_status_success)
        throw CompileException(result, __FILE__, __LINE__);
    // Create shader module
    const char *bytecode = shaderc_result_get_bytes(result);
    const size_t bytecodeSize = shaderc_result_get_length(result);
    std::shared_ptr<ShaderModule> shaderModule;
    try {
        shaderModule = std::make_shared<ShaderModule>(device, reinterpret_cast<const uint32_t *>(bytecode), bytecodeSize);
    } catch (const BadResult& badResult)
    {
        shaderc_result_release(result);
        throw badResult;
    }
    shaderc_result_release(result);
    return shaderModule;
}

CompileException::CompileException(shaderc_compilation_result_t result,
    const char *file, int line):
    Exception(shaderc_result_get_error_message(result), file, line),
    status(shaderc_result_get_compilation_status(result)),
    warnings(shaderc_result_get_num_warnings(result)),
    errors(shaderc_result_get_num_errors(result))
{
    shaderc_result_release(result);
}
} // namespace aux
} // namespace magma
