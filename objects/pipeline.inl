namespace magma
{
#ifdef VK_EXT_pipeline_creation_feedback
inline bool Pipeline::hitPipelineCache() const noexcept
{
    return (creationFeedback.flags & VK_PIPELINE_CREATION_FEEDBACK_APPLICATION_PIPELINE_CACHE_HIT_BIT_EXT);
}

inline bool Pipeline::usedBasePipeline() const noexcept
{
    return (creationFeedback.flags & VK_PIPELINE_CREATION_FEEDBACK_BASE_PIPELINE_ACCELERATION_BIT_EXT);
}

inline uint64_t Pipeline::getCreationDuration() const noexcept
{
    return (creationFeedback.flags & VK_PIPELINE_CREATION_FEEDBACK_VALID_BIT_EXT) ? creationFeedback.duration : 0ull;
}
#endif // VK_EXT_pipeline_creation_feedback

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
