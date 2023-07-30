namespace magma
{
template<class Type>
inline void PipelineBatch::fixup(std::vector<Type>& pipelineInfos) const
{
    collectShaderStageInfos();
    uint32_t offset = 0;
    for (auto& pipelineInfo: pipelineInfos)
    {   // Fixup shader stage pointer
        MAGMA_ASSERT(offset < shaderStageInfos.size());
        pipelineInfo.pStages = &shaderStageInfos[offset];
        offset += pipelineInfo.stageCount;
    }
}
} // namespace magma
