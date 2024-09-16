namespace magma
{
#ifdef VK_EXT_pipeline_creation_feedback
inline bool Pipeline::cacheHit() const noexcept
{
    return creationFeedback.flags & VK_PIPELINE_CREATION_FEEDBACK_APPLICATION_PIPELINE_CACHE_HIT_BIT_EXT;
}

inline bool Pipeline::basePipelineAcceleration() const noexcept
{   // An implementation should set the VK_PIPELINE_CREATION_FEEDBACK_BASE_PIPELINE_ACCELERATION_BIT bit
    // if it was able to avoid a significant amount of work by using the base pipeline.
    return creationFeedback.flags & VK_PIPELINE_CREATION_FEEDBACK_BASE_PIPELINE_ACCELERATION_BIT_EXT;
}

inline uint64_t Pipeline::getCreationDuration() const noexcept
{
    return creationFeedback.flags & VK_PIPELINE_CREATION_FEEDBACK_VALID_BIT_EXT ? creationFeedback.duration : 0ull;
}

inline uint64_t Pipeline::getStageCreationDuration(uint32_t stageIndex) const noexcept
{
    MAGMA_ASSERT(core::countof(stageCreationFeedbacks) == stageCount);
    MAGMA_ASSERT(stageIndex < stageCount);
    if (stageIndex >= stageCount)
        return 0ull;
    // Per-stage feedback may not be provided by an implementation
    const VkPipelineCreationFeedbackEXT& stageCreationFeedback = stageCreationFeedbacks[stageIndex];
    return stageCreationFeedback.flags & VK_PIPELINE_CREATION_FEEDBACK_VALID_BIT_EXT ? stageCreationFeedback.duration : 0ull;
}
#endif // VK_EXT_pipeline_creation_feedback
} // namespace magma
