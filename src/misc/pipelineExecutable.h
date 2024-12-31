/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2024 Victor Coda.

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
#include "../objects/device.h"

namespace magma
{
    /* When a pipeline is created, its state and shaders are
       compiled into zero or more device-specific executables,
       which are used when executing commands against that pipeline.
       This extension adds a mechanism to query properties and
       statistics about the different executables produced by
       the pipeline compilation process. This is intended to be
       used by debugging and performance tools to allow them to
       provide more detailed information to the user. Certain
       compile-time shader statistics provided through this
       extension may be useful to developers for debugging or
       performance analysis. */

#ifdef VK_KHR_pipeline_executable_properties
    class PipelineExecutable : NonCopyable
    {
    public:
        PipelineExecutable(VkDevice device,
            VkPipeline pipeline,
            const VkPipelineExecutablePropertiesKHR& properties,
            uint32_t executableIndex) noexcept;
        const VkPipelineExecutablePropertiesKHR& getProperties() const noexcept { return properties; }
        uint32_t getIndex() const noexcept { return executableIndex; }
        std::vector<VkPipelineExecutableStatisticKHR> getStatistics() const;
        std::vector<VkPipelineExecutableInternalRepresentationKHR> getInternalRepresentations() const;

    private:
        VkDevice getNativeDevice() const noexcept { return device; }

        const VkDevice device;
        const VkPipeline pipeline;
        const VkPipelineExecutablePropertiesKHR properties;
        const uint32_t executableIndex;
        mutable std::vector<std::unique_ptr<char[]>> data;
    };
#endif // VK_KHR_pipeline_executable_properties
} // namespace magma
