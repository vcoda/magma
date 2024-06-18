/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2023 Victor Coda.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <https://www.gnu.org/licenses/>.
*/
#pragma once
#include "../shaders/pipelineShaderStage.h"
#include "pipelineLibrary.h"
#include "../misc/structureChain.h"

namespace magma
{
    class Device;
    class Pipeline;
    class PipelineLayout;
    class PipelineCache;
    class IAllocator;

    /* Base class to batch compilation of pipeline state objects.
       The preferred way is to build pipelines asynchronously in
       a separate thread (which might be a part of a thread pool)
       and wait for completion using future. */

    class PipelineBatch : public IClass
    {
    public:
    #ifdef VK_AMD_pipeline_compiler_control
        void setCompilerControlFlags(VkPipelineCompilerControlFlagsAMD flags) noexcept { compilerControlFlags = flags; }
        VkPipelineCompilerControlFlagsAMD getCompilerControlFlags() noexcept { return compilerControlFlags; }
    #endif // VK_AMD_pipeline_compiler_control
        virtual void buildPipelines(std::shared_ptr<PipelineCache> pipelineCache = nullptr,
        #ifdef VK_KHR_pipeline_library
            std::shared_ptr<PipelineLibrary> pipelineLibrary = nullptr,
        #endif
            std::shared_ptr<IAllocator> allocator = nullptr) = 0;
        std::future<void> buildPipelinesAsync(std::shared_ptr<PipelineCache> pipelineCache = nullptr,
        #ifdef VK_KHR_pipeline_library
            std::shared_ptr<PipelineLibrary> pipelineLibrary = nullptr,
        #endif
            std::shared_ptr<IAllocator> allocator = nullptr);

    protected:
        PipelineBatch(std::shared_ptr<Device> device) noexcept;
        VkDevice getNativeDevice() const noexcept;
        void collectShaderStageInfos() const;

        std::shared_ptr<Device> device;
        std::forward_list<std::vector<PipelineShaderStage>> stages;
        std::forward_list<std::shared_ptr<PipelineLayout>> layouts;
        std::forward_list<std::shared_ptr<Pipeline>> basePipelines;
    #ifdef VK_AMD_pipeline_compiler_control
        std::forward_list<VkPipelineCompilerControlCreateInfoAMD> pipelineCompilerControlInfos;
        VkPipelineCompilerControlFlagsAMD compilerControlFlags;
    #endif
    #ifdef VK_EXT_pipeline_creation_feedback
        std::forward_list<VkPipelineCreationFeedbackEXT> creationFeedbacks;
        std::forward_list<std::vector<VkPipelineCreationFeedbackEXT>> stageCreationFeedbacks;
        std::forward_list<VkPipelineCreationFeedbackCreateInfoEXT> creationFeedbackInfos;
    #endif // VK_EXT_pipeline_creation_feedback
    #ifdef VK_KHR_pipeline_library
        std::forward_list<VkPipelineLibraryCreateInfoKHR> pipelineLibraryInfos;
    #endif
        std::forward_list<hash_t> hashes;
        mutable std::vector<VkPipelineShaderStageCreateInfo> shaderStageInfos;
    };

    /* Template base pipeline batch that holds an arrays of
       create descriptors and handles of pipeline objects. */

    template<class PipelineType, class PipelineCreateInfo>
    class BasePipelineBatch : public PipelineBatch
    {
    public:
        uint32_t getPipelineCount() const noexcept { return MAGMA_COUNT(pipelines); }
        const std::shared_ptr<PipelineType>& getPipeline(uint32_t index) const noexcept { return pipelines[index]; }

    protected:
        BasePipelineBatch(std::shared_ptr<Device> device) noexcept;
        void fixup() noexcept;
    #ifdef VK_KHR_pipeline_library
        void linkPipelineLibrary(std::shared_ptr<PipelineLibrary> pipelineLibrary);
    #endif // VK_KHR_pipeline_library
        void postCreate();
        void postBuild();

        std::vector<PipelineCreateInfo> pipelineInfos; // Use vector to preserve order
        std::deque<std::shared_ptr<PipelineType>> pipelines;
    };
} // namespace magma

#include "pipelineBatch.inl"
