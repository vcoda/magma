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
#include "../misc/structureChain.h"

namespace magma
{
    class Device;
    class Pipeline;
    class PipelineLayout;
    class PipelineCache;
    class IAllocator;

    /* Base class to batch compilation of pipeline state objects.
       The preferred way is to build pipelines asynchronously
       in a separate thread (which might be a part of a thread pool)
       and wait for completion using future. */

    class PipelineBatch : public IDestructible
    {
    public:
        virtual void buildPipelines(std::shared_ptr<Device> device,
            std::shared_ptr<PipelineCache> pipelineCache = nullptr,
            std::shared_ptr<IAllocator> allocator = nullptr) = 0;
        std::future<void> buildPipelinesAsync(std::shared_ptr<Device> device,
            std::shared_ptr<PipelineCache> pipelineCache = nullptr,
            std::shared_ptr<IAllocator> allocator = nullptr);
    #ifdef VK_AMD_pipeline_compiler_control
        static void setCompilerControlFlags(VkPipelineCompilerControlFlagsAMD flags) noexcept { compilerControlFlags = flags; }
        static VkPipelineCompilerControlFlagsAMD getCompilerControlFlags() noexcept { return compilerControlFlags; }
    #endif // VK_AMD_pipeline_compiler_control

    protected:
        void collectShaderStageInfos() const;

        std::list<std::vector<PipelineShaderStage>> stages;
        std::list<std::shared_ptr<PipelineLayout>> layouts;
        std::list<std::shared_ptr<Pipeline>> basePipelines;
    #ifdef VK_AMD_pipeline_compiler_control
        std::list<VkPipelineCompilerControlCreateInfoAMD> pipelineCompilerControlInfos;
        static VkPipelineCompilerControlFlagsAMD compilerControlFlags;
    #endif
    #ifdef VK_EXT_pipeline_creation_feedback
        std::list<VkPipelineCreationFeedbackEXT> creationFeedbacks;
        std::list<std::vector<VkPipelineCreationFeedbackEXT>> stageCreationFeedbacks;
        std::list<VkPipelineCreationFeedbackCreateInfoEXT> creationFeedbackInfos;
    #endif // VK_EXT_pipeline_creation_feedback
        std::vector<hash_t> hashes;
        mutable std::vector<VkPipelineShaderStageCreateInfo> shaderStageInfos;
    };

    template<class PipelineType>
    class TPipelineBatch : public PipelineBatch
    {
    public:
        uint32_t getPipelineCount() const noexcept { return MAGMA_COUNT(pipelines); }
        const std::shared_ptr<PipelineType>& getPipeline(uint32_t index) const noexcept { return pipelines[index]; }

    protected:
        TPipelineBatch(uint32_t capacity);
        template<class PipelineInfoType>
        void fixup(std::vector<PipelineInfoType>& pipelineInfos) const noexcept;
        void postCreate();
        void postBuild();

        std::vector<std::shared_ptr<PipelineType>> pipelines;
    };
} // namespace magma

#include "pipelineBatch.inl"
