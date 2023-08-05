/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2022 Victor Coda.

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
#include "nondispatchable.h"

namespace magma
{
    class Device;
    class PipelineLayout;
    class PipelineExecutable;
    class IAllocator;

    /* Some Vulkan commands specify geometric objects to be drawn or computational work to be performed,
       while others specify state controlling how objects are handled by the various pipeline stages,
       or control data transfer between memory organized as images and buffers. Commands are effectively sent
       through a processing pipeline, either a graphics pipeline or a compute pipeline. */

    class Pipeline : public NonDispatchable<VkPipeline>,
        public std::enable_shared_from_this<Pipeline>
    {
    public:
        ~Pipeline();
        VkPipelineBindPoint getBindPoint() const noexcept { return bindPoint; }
        uint32_t getStageCount() const noexcept { return stageCount; }
        std::shared_ptr<PipelineLayout> getLayout() noexcept { return layout; }
        std::shared_ptr<const PipelineLayout> getLayout() const noexcept { return layout; }
        std::shared_ptr<Pipeline> getBasePipeline() noexcept { return basePipeline; }
        std::shared_ptr<const Pipeline> getBasePipeline() const noexcept { return basePipeline; }
        hash_t getHash() const noexcept { return hash; }
    #ifdef VK_KHR_pipeline_executable_properties
        std::vector<std::shared_ptr<PipelineExecutable>> getExecutables() const;
    #endif // VK_KHR_pipeline_executable_properties
    #ifdef VK_AMD_shader_info
        VkShaderStatisticsInfoAMD getShaderStatistics(VkShaderStageFlagBits stage) const;
        std::vector<uint8_t> getShaderBinary(VkShaderStageFlagBits stage) const;
        std::string getShaderDisassembly(VkShaderStageFlagBits stage) const;
    #endif // VK_AMD_shader_info
    #ifdef VK_EXT_pipeline_creation_feedback
        bool hitPipelineCache() const noexcept;
        bool usedBasePipeline() const noexcept;
        uint64_t getCreationDuration() const noexcept;
        uint64_t getStageCreationDuration(uint32_t stageIndex) const noexcept;
    #endif // VK_EXT_pipeline_creation_feedback

    protected:
        Pipeline(VkPipelineBindPoint bindPoint,
            std::shared_ptr<Device> device,
            std::shared_ptr<PipelineLayout> layout,
            std::shared_ptr<Pipeline> basePipeline,
            std::shared_ptr<IAllocator> allocator,
            uint32_t stageCount,
        #ifdef VK_EXT_pipeline_creation_feedback
            VkPipelineCreationFeedbackEXT creationFeedback = {},
            const std::vector<VkPipelineCreationFeedbackEXT>& stageCreationFeedbacks = {},
        #endif
            hash_t hash = 0ull);

        const VkPipelineBindPoint bindPoint;
        const uint32_t stageCount;
        std::shared_ptr<PipelineLayout> layout;
        std::shared_ptr<Pipeline> basePipeline;
    #ifdef VK_EXT_pipeline_creation_feedback
        VkPipelineCreationFeedbackEXT creationFeedback;
        std::vector<VkPipelineCreationFeedbackEXT> stageCreationFeedbacks;
    #endif // VK_EXT_pipeline_creation_feedback
        hash_t hash;
    };
} // namespace magma

#include "pipeline.inl"
