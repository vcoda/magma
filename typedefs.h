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

namespace magma
{
    typedef std::shared_ptr<Buffer> BufferPtr;
    typedef std::shared_ptr<BufferView> BufferViewPtr;
    typedef std::shared_ptr<CommandBuffer> CommandBufferPtr;
    typedef std::shared_ptr<CommandPool> CommandPoolPtr;
    typedef std::shared_ptr<DebugReportCallback> DebugReportCallbackPtr;
    typedef std::shared_ptr<DescriptorPool> DescriptorPoolPtr;
    typedef std::shared_ptr<DescriptorSet> DescriptorSetPtr;
    typedef std::shared_ptr<DescriptorSetLayout> DescriptorSetLayoutPtr;
    typedef std::shared_ptr<Device> DevicePtr;
    typedef std::shared_ptr<DeviceMemory> DeviceMemoryPtr;
    typedef std::shared_ptr<Event> EventPtr;
    typedef std::shared_ptr<Fence> FencePtr;
    typedef std::shared_ptr<Framebuffer> FramebufferPtr;
    typedef std::shared_ptr<Image> ImagePtr;
    typedef std::shared_ptr<Image1D> Image1DPtr;
    typedef std::shared_ptr<Image1DArray> Image1DArrayPtr;
    typedef std::shared_ptr<Image2D> Image2DPtr;
    typedef std::shared_ptr<Image2DArray> Image2DArrayPtr;
    typedef std::shared_ptr<Image3D> Image3DPtr;
    typedef std::shared_ptr<ImageCube> ImageCubePtr;
    typedef std::shared_ptr<ImageView> ImageViewPtr;
    typedef std::shared_ptr<IndexBuffer> IndexBufferPtr;
    typedef std::shared_ptr<IndirectBuffer> IndirectBufferPtr;
    typedef std::shared_ptr<Instance> InstancePtr;
    typedef std::shared_ptr<PhysicalDevice> PhysicalDevicePtr;
    typedef std::shared_ptr<Pipeline> PipelinePtr;
    typedef std::shared_ptr<PipelineCache> PipelineCachePtr;
    typedef std::shared_ptr<PipelineLayout> PipelineLayoutPtr;
    typedef std::shared_ptr<QueryPool> QueryPoolPtr;
    typedef std::shared_ptr<Queue> QueuePtr;
    typedef std::shared_ptr<RenderPass> RenderPassPtr;
    typedef std::shared_ptr<Sampler> SamplerPtr;
    typedef std::shared_ptr<Semaphore> SemaphorePtr;
    typedef std::shared_ptr<ShaderModule> ShaderModulePtr;
    typedef std::shared_ptr<StorageBuffer> StorageBufferPtr;
    typedef std::shared_ptr<Surface> SurfacePtr;
    typedef std::shared_ptr<Swapchain> SwapchainPtr;
    typedef std::shared_ptr<SourceTransferBuffer> SourceTransferBufferPtr;
    typedef std::shared_ptr<DestTransferBuffer> DestTransferBufferPtr;
    template<class Block> using UniformBufferPtr = std::shared_ptr<UniformBuffer<Block>>;
    typedef std::shared_ptr<VertexBuffer> VertexBufferPtr;
} // namespace magma
