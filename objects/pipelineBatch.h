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
#include "../core/destructible.h"
#include "../core/noncopyable.h"
#include "../shaders/pipelineShaderStage.h"

namespace magma
{
    class Device;
    class Pipeline;
    class PipelineLayout;
    class PipelineCache;
    class IAllocator;

    /* Base class for batching pipeline construction. The preferred
       way is to build pipelines asynchronously in a separate thread
       (which might be a part of a thread pool) and wait for completion
       using std::future. */

    template<class PipelineType>
    class PipelineBatch : public core::IDestructible,
        /* private */ core::NonCopyable
    {
    public:
        virtual void buildPipelines(std::shared_ptr<Device> device,
            std::shared_ptr<PipelineCache> pipelineCache = nullptr,
            std::shared_ptr<IAllocator> allocator = nullptr) = 0;
        std::future<void> buildPipelinesAsync(std::shared_ptr<Device> device,
            std::shared_ptr<PipelineCache> pipelineCache = nullptr,
            std::shared_ptr<IAllocator> allocator = nullptr);
        uint32_t getPipelineCount() const noexcept { return MAGMA_COUNT(pipelines); }
        const std::shared_ptr<PipelineType>& getPipeline(uint32_t index) const noexcept { return pipelines[index]; }

    protected:
        template<class PipelineInfoType>
        void fixup(std::vector<PipelineInfoType>& pipelineInfos) const noexcept;
        void postCreate();
        void postBuild();

        std::vector<std::shared_ptr<PipelineType>> pipelines;
        std::list<std::vector<PipelineShaderStage>> stages;
        std::list<std::shared_ptr<PipelineLayout>> layouts;
        std::list<std::shared_ptr<Pipeline>> basePipelines;
    #ifdef VK_EXT_pipeline_creation_feedback
        std::list<VkPipelineCreationFeedbackEXT> creationFeedbacks;
        std::list<std::vector<VkPipelineCreationFeedbackEXT>> stageCreationFeedbacks;
        std::list<VkPipelineCreationFeedbackCreateInfoEXT> creationFeedbackInfos;
    #endif // VK_EXT_pipeline_creation_feedback
        std::list<hash_t> hashes;

    private:
        void collectShaderStageInfos() const;

        mutable std::vector<VkPipelineShaderStageCreateInfo> shaderStageInfos;
    };
} // namespace magma

#include "pipelineBatch.inl"
