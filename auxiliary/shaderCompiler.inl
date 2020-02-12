namespace magma
{
namespace aux
{
inline void ShaderCompiler::setOptimizationLevel(shaderc_optimization_level optimizationLevel) noexcept
{
    this->optimizationLevel = optimizationLevel;
}

inline void ShaderCompiler::setGenerateDebugInfo(bool generateDebugInfo) noexcept
{
    this->generateDebugInfo = generateDebugInfo;
}

inline void ShaderCompiler::setSuppressWarnings(bool suppressWarnings) noexcept
{
    this->suppressWarnings = suppressWarnings;
}

inline void ShaderCompiler::setWarningsAsErrors(bool warningsAsErrors) noexcept
{
    this->warningsAsErrors = warningsAsErrors;
}
} // namespace aux
} // namespace magma
