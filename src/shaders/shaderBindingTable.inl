namespace magma
{
#ifdef VK_KHR_ray_tracing_pipeline
template<class Block>
inline void ShaderBindingTable::addShaderRecord(VkShaderStageFlagBits stage, uint32_t groupIndex, const Block& block)
{
    groups[stage].shaderRecords[groupIndex] = ShaderRecord(block);
}
#endif // VK_KHR_ray_tracing_pipeline
} // namespace magma
