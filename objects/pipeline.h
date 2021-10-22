/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2020 Victor Coda.

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
#include "pipelineLayout.h"
#include "../shaders/pipelineShaderStage.h"

namespace magma
{
    class Device;
    class PipelineCache;
    class IAllocator;

    /* Some Vulkan commands specify geometric objects to be drawn or computational work to be performed,
       while others specify state controlling how objects are handled by the various pipeline stages,
       or control data transfer between memory organized as images and buffers. Commands are effectively sent
       through a processing pipeline, either a graphics pipeline or a compute pipeline. */

    class Pipeline : public NonDispatchable<VkPipeline>
    {
    public:
        ~Pipeline();
        VkPipelineBindPoint getBindPoint() const noexcept { return bindPoint; }
        std::shared_ptr<PipelineLayout> getLayout() noexcept { return layout; }
        std::shared_ptr<const PipelineLayout> getLayout() const noexcept { return layout; }
        std::shared_ptr<Pipeline> getBasePipeline() noexcept { return basePipeline; }
        std::shared_ptr<const Pipeline> getBasePipeline() const noexcept { return basePipeline; }
        std::size_t getHash() const noexcept { return hash; }
#ifdef VK_AMD_shader_info
        VkShaderStatisticsInfoAMD getShaderStatistics(VkShaderStageFlagBits stage) const;
        std::vector<uint8_t> getShaderBinary(VkShaderStageFlagBits stage) const;
        std::string getShaderDisassembly(VkShaderStageFlagBits stage) const;
#endif // VK_AMD_shader_info

    protected:
        explicit Pipeline(VkPipelineBindPoint bindPoint,
            std::shared_ptr<Device> device,
            std::shared_ptr<PipelineLayout> layout,
            std::shared_ptr<Pipeline> basePipeline,
            std::shared_ptr<IAllocator> allocator);

        const VkPipelineBindPoint bindPoint;
        std::shared_ptr<PipelineLayout> layout;
        std::shared_ptr<Pipeline> basePipeline;
        std::size_t hash = 0;
    };

    /* Vulkan has an ability to create multiple pipeline objects in a single call.
       This is a base class for such implementation. */

    class Pipelines : public core::NonCopyable
    {
    protected:
        template<typename Type>
        void fixup(std::vector<Type>& pipelineInfos) const;

        std::list<std::vector<PipelineShaderStage>> stages;
        std::list<std::shared_ptr<PipelineLayout>> layouts;
        std::list<std::shared_ptr<Pipeline>> basePipelines;
        std::list<std::size_t> hashes;

    private:
        void gatherShaderStageInfos() const;
        mutable std::vector<VkPipelineShaderStageCreateInfo> shaderStageInfos;
    };
} // namespace magma

#include "pipeline.inl"
