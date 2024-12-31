/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2023 Victor Coda.

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
#include "pch.h"
#pragma hdrstop
#include "shaderCompiler.h"
#include "../objects/device.h"
#include "../objects/shaderModule.h"
#include "../exceptions/errorResult.h"
#include "../exceptions/compileError.h"

namespace magma::aux
{
ShaderCompiler::ShaderCompiler(std::shared_ptr<Device> device, std::shared_ptr<IShaderInclude> handler):
    device(std::move(device)),
    includeHandler(std::move(handler))
{
    compiler = shaderc_compiler_initialize();
    if (!compiler)
        MAGMA_ERROR("failed to initialize shader compiler");
}

ShaderCompiler::~ShaderCompiler()
{
    shaderc_compiler_release(compiler);
}

std::shared_ptr<ShaderModule> ShaderCompiler::compileShader(const std::string& source, const char *entrypoint,
    shaderc_shader_kind shaderKind /* shaderc_glsl_infer_from_source */,
    const std::unordered_map<std::string, std::string>& macroDefinitions /* {} */,
    const std::string& srcFileName  /* "" */)
{
    MAGMA_ASSERT(source.length());
    MAGMA_ASSERT(strlen(entrypoint));
    shaderc_compile_options_t options = shaderc_compile_options_initialize();
    for (auto const& [name, value]: macroDefinitions)
    {   // Add preprocessor definitions
        shaderc_compile_options_add_macro_definition(options,
            name.c_str(), name.length(),
            value.c_str(), value.length());
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
    {
    #ifndef MAGMA_NO_EXCEPTIONS
        throw exception::CompileError(result, MAGMA_SOURCE_LOCATION);
    #else
        MAGMA_ASSERT(shaderc_compilation_status_success == status);
        return nullptr;
    #endif // MAGMA_NO_EXCEPTIONS
    }
    // Create shader module
    const char *bytecode = shaderc_result_get_bytes(result);
    const std::size_t bytecodeSize = shaderc_result_get_length(result);
    std::shared_ptr<ShaderModule> shaderModule = std::make_shared<ShaderModule>(device,
        reinterpret_cast<const uint32_t *>(bytecode), bytecodeSize, 0, device->getHostAllocator(), false);
    shaderc_result_release(result);
    return shaderModule;
}
} // namespace magma::aux
