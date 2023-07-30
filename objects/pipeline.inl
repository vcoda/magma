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
} // namespace magma
