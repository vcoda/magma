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

namespace magma
{
    MAGMA_TYPEDEF_SHARED_PTR(Buffer)
    MAGMA_TYPEDEF_SHARED_PTR(BufferView)
    MAGMA_TYPEDEF_SHARED_PTR(ColorAttachment2D)
    MAGMA_TYPEDEF_SHARED_PTR(CommandBuffer)
    MAGMA_TYPEDEF_SHARED_PTR(CommandPool)
    MAGMA_TYPEDEF_SHARED_PTR(CompactedSizeQuery)
    MAGMA_TYPEDEF_SHARED_PTR(ComputePipeline)
    MAGMA_TYPEDEF_SHARED_PTR(ConditionalRenderingBuffer)
    MAGMA_TYPEDEF_SHARED_PTR(DebugReportCallback)
    MAGMA_TYPEDEF_SHARED_PTR(DebugUtilsMessenger)
    MAGMA_TYPEDEF_SHARED_PTR(DepthStencilAttachment2D)
    MAGMA_TYPEDEF_SHARED_PTR(DescriptorPool)
    MAGMA_TYPEDEF_SHARED_PTR(DescriptorSet)
    MAGMA_TYPEDEF_SHARED_PTR(DescriptorSetLayout)
    MAGMA_TYPEDEF_SHARED_PTR(Device)
    MAGMA_TYPEDEF_SHARED_PTR(DeviceMemory)
    MAGMA_TYPEDEF_SHARED_PTR(DisplayMode)
    MAGMA_TYPEDEF_SHARED_PTR(DisplaySurface)
    MAGMA_TYPEDEF_SHARED_PTR(DstTransferBuffer)
    MAGMA_TYPEDEF_SHARED_PTR(DynamicIndexBuffer)
    MAGMA_TYPEDEF_SHARED_PTR(DynamicStorageBuffer)
    MAGMA_TYPEDEF_SHARED_PTR(DynamicStorageTexelBuffer)
    MAGMA_TYPEDEF_SHARED_PTR(DynamicVertexBuffer)
    MAGMA_TYPEDEF_SHARED_PTR(Event)
    MAGMA_TYPEDEF_SHARED_PTR(Fence)
    MAGMA_TYPEDEF_SHARED_PTR(Framebuffer)
    MAGMA_TYPEDEF_SHARED_PTR(GraphicsPipeline)
    MAGMA_TYPEDEF_SHARED_PTR(Image)
    MAGMA_TYPEDEF_SHARED_PTR(Image1D)
    MAGMA_TYPEDEF_SHARED_PTR(Image1DArray)
    MAGMA_TYPEDEF_SHARED_PTR(Image2D)
    MAGMA_TYPEDEF_SHARED_PTR(Image2DArray)
    MAGMA_TYPEDEF_SHARED_PTR(Image3D)
    MAGMA_TYPEDEF_SHARED_PTR(ImageCube)
    MAGMA_TYPEDEF_SHARED_PTR(ImageView)
    MAGMA_TYPEDEF_SHARED_PTR(IndexBuffer)
    MAGMA_TYPEDEF_SHARED_PTR(IndirectBuffer)
    MAGMA_TYPEDEF_SHARED_PTR(Instance)
    MAGMA_TYPEDEF_SHARED_PTR(OcclusionQuery)
    MAGMA_TYPEDEF_SHARED_PTR(PhysicalDevice)
    MAGMA_TYPEDEF_SHARED_PTR(PhysicalDeviceGroup)
    MAGMA_TYPEDEF_SHARED_PTR(Pipeline)
    MAGMA_TYPEDEF_SHARED_PTR(PipelineCache)
    MAGMA_TYPEDEF_SHARED_PTR(PipelineLayout)
    MAGMA_TYPEDEF_SHARED_PTR(PipelineStatisticsQuery)
    MAGMA_TYPEDEF_SHARED_PTR(RayTracingBuffer)
    MAGMA_TYPEDEF_SHARED_PTR(RayTracingPipeline)
    MAGMA_TYPEDEF_SHARED_PTR(QueryPool)
    MAGMA_TYPEDEF_SHARED_PTR(Queue)
    MAGMA_TYPEDEF_SHARED_PTR(RenderPass)
    MAGMA_TYPEDEF_SHARED_PTR(Sampler)
    MAGMA_TYPEDEF_SHARED_PTR(Semaphore)
    MAGMA_TYPEDEF_SHARED_PTR(ShaderModule)
    MAGMA_TYPEDEF_SHARED_PTR(SrcTransferBuffer)
    MAGMA_TYPEDEF_SHARED_PTR(StorageBuffer)
    MAGMA_TYPEDEF_SHARED_PTR(StorageTexelBuffer)
    MAGMA_TYPEDEF_SHARED_PTR(Surface)
    MAGMA_TYPEDEF_SHARED_PTR(Swapchain)
    MAGMA_TYPEDEF_SHARED_PTR(SwapchainColorAttachment2D)
    MAGMA_TYPEDEF_SHARED_PTR(TimestampQuery)
    MAGMA_TYPEDEF_SHARED_PTR(UniformTexelBuffer)
    MAGMA_TYPEDEF_SHARED_PTR(ValidationCache)
    MAGMA_TYPEDEF_SHARED_PTR(VertexBuffer)

    MAGMA_TYPEDEF_SHARED_PTR(Win32Surface)
    MAGMA_TYPEDEF_SHARED_PTR(XlibSurface)
    MAGMA_TYPEDEF_SHARED_PTR(XcbSurface)
    MAGMA_TYPEDEF_SHARED_PTR(WaylandSurface)
    MAGMA_TYPEDEF_SHARED_PTR(MirSurface)
    MAGMA_TYPEDEF_SHARED_PTR(AndroidSurface)
    MAGMA_TYPEDEF_SHARED_PTR(iOSSurface)
    MAGMA_TYPEDEF_SHARED_PTR(MacOSSurface)
    MAGMA_TYPEDEF_SHARED_PTR(MetalSurface)

    MAGMA_TYPEDEF_INTERFACE_SHARED_PTR(Allocator)

    MAGMA_TYPEDEF_UNIQUE_PTR(InstanceExtensions)
    MAGMA_TYPEDEF_UNIQUE_PTR(PhysicalDeviceExtensions)

    class Display; // Declare explicitly because of Xlib's Diplay
    typedef std::shared_ptr<Display> DisplayPtr;

    typedef std::function<void *(void *, const void *, std::size_t)> CopyMemoryFunction;
    typedef std::function<void *(void *, std::size_t)> ZeroMemoryFunction;
} // namespace magma
