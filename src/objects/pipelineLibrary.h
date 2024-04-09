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

namespace magma
{
    class Device;
    class PipelineLayout;
    class PipelineShaderStage;
    class IAllocator;

    class VertexInputState;
    struct InputAssemblyState;

    /* A pipeline library is a special pipeline that cannot be bound,
       instead it defines a set of shaders and shader groups which
       can be linked into other pipelines. */

#ifdef VK_KHR_pipeline_library
    class PipelineLibrary : public IDestructible
    {
    public:
        explicit PipelineLibrary(std::shared_ptr<Device> device,
            std::shared_ptr<IAllocator> hostAllocator = nullptr) noexcept;
        ~PipelineLibrary();
    #ifdef VK_EXT_graphics_pipeline_library
        void compileVertexInputInterface(const VertexInputState& vertexInputState,
            const InputAssemblyState& inputAssemblyState);
    #endif // VK_EXT_graphics_pipeline_library
        uint32_t getLibraryCount() const noexcept { return MAGMA_COUNT(libraries); }
        const VkPipeline *getLibraries() const noexcept { return libraries.data(); }

    protected:
        std::shared_ptr<Device> device;
        std::shared_ptr<IAllocator> hostAllocator;
        std::vector<VkPipeline> libraries;
    };
#endif // VK_KHR_pipeline_library
} // namespace magma
