/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2019 Victor Coda.

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
        std::shared_ptr<PipelineCache> getPipelineCache() noexcept { return pipelineCache; }
		std::shared_ptr<const PipelineCache> getPipelineCache() const noexcept { return pipelineCache; }
        std::shared_ptr<Pipeline> getBasePipeline() noexcept { return basePipeline; }
		std::shared_ptr<const Pipeline> getBasePipeline() const noexcept { return basePipeline; }
        std::size_t getHash() const noexcept { return hash; }
#ifdef VK_AMD_shader_info
        VkShaderStatisticsInfoAMD getShaderStatistics(VkShaderStageFlagBits stage) const;
        std::vector<uint8_t> getShaderBinary(VkShaderStageFlagBits stage) const;
        std::string getShaderDisassembly(VkShaderStageFlagBits stage) const;
#endif

    protected:
        explicit Pipeline(VkPipelineBindPoint bindPoint,
            std::shared_ptr<Device> device,
            std::shared_ptr<PipelineLayout> layout,
            std::shared_ptr<PipelineCache> pipelineCache,
            std::shared_ptr<Pipeline> basePipeline,
            std::shared_ptr<IAllocator> allocator);

        VkPipelineBindPoint bindPoint;
        std::shared_ptr<PipelineLayout> layout;
        std::shared_ptr<PipelineCache> pipelineCache;
        std::shared_ptr<Pipeline> basePipeline;
        std::size_t hash = 0;
    };
} // namespace magma
