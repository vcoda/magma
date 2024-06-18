namespace magma
{
constexpr RayTracingShaderGroup::RayTracingShaderGroup(VkRayTracingShaderGroupTypeKHR type,
    uint32_t generalShader, uint32_t closestHitShader, uint32_t anyHitShader, uint32_t intersectionShader,
    const void *captureReplayHandle) noexcept:
    VkRayTracingShaderGroupCreateInfoKHR{
        VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_KHR,
        nullptr,
        type,
        generalShader,
        closestHitShader,
        anyHitShader,
        intersectionShader,
        captureReplayHandle
    }
{}

constexpr hash_t RayTracingShaderGroup::hash() const noexcept
{
    return core::hashArgs(
        sType,
        type,
        generalShader,
        closestHitShader,
        anyHitShader,
        intersectionShader,
        pShaderGroupCaptureReplayHandle);
}

constexpr GeneralRayTracingShaderGroup::GeneralRayTracingShaderGroup(uint32_t generalShader,
    const void *captureReplayHandle /* nullptr */) noexcept:
    RayTracingShaderGroup(VK_RAY_TRACING_SHADER_GROUP_TYPE_GENERAL_KHR,
        generalShader, VK_SHADER_UNUSED_KHR, VK_SHADER_UNUSED_KHR, VK_SHADER_UNUSED_KHR,
        captureReplayHandle)
{}

constexpr TrianglesHitRayTracingShaderGroup::TrianglesHitRayTracingShaderGroup(uint32_t closestHitShader,
    uint32_t anyHitShader /* VK_SHADER_UNUSED_KHR */,
    const void *captureReplayHandle /* nullptr */) noexcept:
    RayTracingShaderGroup(VK_RAY_TRACING_SHADER_GROUP_TYPE_TRIANGLES_HIT_GROUP_KHR,
        VK_SHADER_UNUSED_KHR, closestHitShader, anyHitShader, VK_SHADER_UNUSED_KHR,
        captureReplayHandle)
{}

constexpr ProceduralHitRayTracingShaderGroup::ProceduralHitRayTracingShaderGroup(uint32_t intersectionShader,
    uint32_t closestHitShader /* VK_SHADER_UNUSED_KHR */,
    uint32_t anyHitShader /* VK_SHADER_UNUSED_KHR */,
    const void *captureReplayHandle /* nullptr */) noexcept:
    RayTracingShaderGroup(VK_RAY_TRACING_SHADER_GROUP_TYPE_PROCEDURAL_HIT_GROUP_KHR,
        VK_SHADER_UNUSED_KHR, closestHitShader, anyHitShader, intersectionShader,
        captureReplayHandle)
{}
} // namespace magma
