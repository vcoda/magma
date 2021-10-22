namespace magma
{
template<typename Type>
inline void Pipelines::fixup(std::vector<Type>& pipelineInfos) const
{
    gatherShaderStageInfos();
    uint32_t offset = 0;
    for (auto& pipelineInfo : pipelineInfos)
    {   // Fixup shader stage pointer
        MAGMA_ASSERT(offset < shaderStageInfos.size());
        pipelineInfo.pStages = &shaderStageInfos[offset];
        offset += pipelineInfo.stageCount;
    }
}
} // namespace magma
