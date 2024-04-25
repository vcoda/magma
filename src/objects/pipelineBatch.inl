namespace magma
{
template<class PipelineType>
inline TPipelineBatch<PipelineType>::TPipelineBatch(std::shared_ptr<Device> device) noexcept:
    PipelineBatch(std::move(device))
{}

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

#ifdef VK_KHR_pipeline_library
template<class PipelineType>
template<class PipelineInfoType>
inline void TPipelineBatch<PipelineType>::linkPipelineLibrary(std::vector<PipelineInfoType>& pipelineInfos,
    std::shared_ptr<PipelineLibrary> pipelineLibrary)
{
    if (pipelineLibrary)
    {
        VkPipelineLibraryCreateInfoKHR pipelineLibraryInfo;
        pipelineLibraryInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LIBRARY_CREATE_INFO_KHR;
        pipelineLibraryInfo.pNext = nullptr;
        pipelineLibraryInfo.libraryCount = pipelineLibrary->getLibraryCount();
        pipelineLibraryInfo.pLibraries = pipelineLibrary->getLibraries();
        for (auto& pipelineInfo: pipelineInfos)
        {   // For each pipeline info we link a new instance of library info
            pipelineLibraryInfos.push_front(pipelineLibraryInfo);
            linkNode(pipelineInfo, pipelineLibraryInfos.front());
        }
    }
}
#endif // VK_KHR_pipeline_library

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
#ifdef VK_KHR_pipeline_library
    pipelineLibraryInfos.clear();
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
