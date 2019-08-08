/*
Magma - C++1x interface over Khronos Vulkan API.
Copyright (C) 2018 Victor Coda.

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
#include "pipeline.h"
#include "../shaders/pipelineShaderStage.h"

namespace magma
{
    class PipelineCache;

    class ComputePipeline : public Pipeline
    {
    public:
        explicit ComputePipeline(std::shared_ptr<Device> device,
            std::shared_ptr<PipelineCache> cache,
            const PipelineShaderStage& stage,
            std::shared_ptr<PipelineLayout> layout = nullptr,
            std::shared_ptr<ComputePipeline> basePipeline = nullptr,
            VkPipelineCreateFlags flags = 0,
            std::shared_ptr<IAllocator> allocator = nullptr);
    };
} // namespace magma
