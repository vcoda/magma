namespace magma
{
template<class PipelineType>
template<class PipelineInfoType>
inline void TPipelineBatch<PipelineType>::fixup(std::vector<PipelineInfoType>& pipelineInfos) const noexcept
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
inline void TPipelineBatch<PipelineType>::postCreate()
{   // Free storage that had to be preserved until vkCreate*Pipelines() call
    pipelines.clear();
#ifdef VK_AMD_pipeline_compiler_control
    pipelineCompilerControlInfos.clear();
#endif
#ifdef VK_EXT_pipeline_creation_feedback
    creationFeedbackInfos.clear();
#endif
}

template<class PipelineType>
inline void TPipelineBatch<PipelineType>::postBuild()
{   // Free storage that had to be preserved until objects are constructed
    stages.clear();
    layouts.clear();
    basePipelines.clear();
#ifdef VK_EXT_pipeline_creation_feedback
    creationFeedbacks.clear();
#endif
    hashes.clear();
}
} // namespace magma
