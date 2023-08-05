namespace magma
{
template<class PipelineType>
template<class PipelineInfoType>
inline void PipelineBatch<PipelineType>::fixup(std::vector<PipelineInfoType>& pipelineInfos) const noexcept
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

template<class PipelineType>
inline void PipelineBatch<PipelineType>::postCreate()
{   // Free storage that had to be preserved until vkCreate*Pipelines() call
    pipelines.clear();
#ifdef VK_EXT_pipeline_creation_feedback
    creationFeedbackInfos.clear();
#endif
}

template<class PipelineType>
inline void PipelineBatch<PipelineType>::postBuild()
{   // Free storage that had to be preserved until objects are constructed
    stages.clear();
    layouts.clear();
    basePipelines.clear();
#ifdef VK_EXT_pipeline_creation_feedback
    creationFeedbacks.clear();
#endif
    hashes.clear();
}

template<class PipelineType>
inline void PipelineBatch<PipelineType>::collectShaderStageInfos() const
{
    std::size_t stageCount = 0;
    for (const auto& shaderStages: stages)
        stageCount += shaderStages.size();
    shaderStageInfos.clear();
    shaderStageInfos.reserve(stageCount);
    for (const auto& shaderStages: stages)
    {   // Copy to array of Vulkan structures due to alignment
        for (const auto& stage: shaderStages)
            shaderStageInfos.push_back(stage);
    }
}
} // namespace magma
