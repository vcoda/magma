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

namespace magma
{
    MAGMA_TYPEDEF_SHARED_PTR(Buffer)
    MAGMA_TYPEDEF_SHARED_PTR(BufferView)
    MAGMA_TYPEDEF_SHARED_PTR(ColorAttachment)
    MAGMA_TYPEDEF_SHARED_PTR(CommandBuffer)
    MAGMA_TYPEDEF_SHARED_PTR(CommandPool)
    MAGMA_TYPEDEF_SHARED_PTR(ComputePipeline)
    MAGMA_TYPEDEF_SHARED_PTR(ComputePipelines)
    MAGMA_TYPEDEF_SHARED_PTR(DeferredOperation)
    MAGMA_TYPEDEF_SHARED_PTR(DepthStencilAttachment)
    MAGMA_TYPEDEF_SHARED_PTR(DescriptorPool)
    MAGMA_TYPEDEF_SHARED_PTR(DescriptorSet)
    MAGMA_TYPEDEF_SHARED_PTR(DescriptorSetLayout)
    MAGMA_TYPEDEF_SHARED_PTR(Device)
    MAGMA_TYPEDEF_SHARED_PTR(DeviceMemory)
    MAGMA_TYPEDEF_SHARED_PTR(DrawIndexedIndirectBuffer)
    MAGMA_TYPEDEF_SHARED_PTR(DrawIndirectBuffer)
    MAGMA_TYPEDEF_SHARED_PTR(DstTransferBuffer)
    MAGMA_TYPEDEF_SHARED_PTR(DynamicIndexBuffer)
    MAGMA_TYPEDEF_SHARED_PTR(DynamicStorageBuffer)
    MAGMA_TYPEDEF_SHARED_PTR(DynamicStorageTexelBuffer)
    MAGMA_TYPEDEF_SHARED_PTR(DynamicVertexBuffer)
    MAGMA_TYPEDEF_SHARED_PTR(Event)
    MAGMA_TYPEDEF_SHARED_PTR(Fence)
    MAGMA_TYPEDEF_SHARED_PTR(Framebuffer)
    MAGMA_TYPEDEF_SHARED_PTR(GraphicsPipeline)
    MAGMA_TYPEDEF_SHARED_PTR(GraphicsPipelines)
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
    MAGMA_TYPEDEF_SHARED_PTR(LinearTiledImage2D)
    MAGMA_TYPEDEF_SHARED_PTR(LodSampler)
    MAGMA_TYPEDEF_SHARED_PTR(OcclusionQuery)
    MAGMA_TYPEDEF_SHARED_PTR(PhysicalDevice)
    MAGMA_TYPEDEF_SHARED_PTR(Pipeline)
    MAGMA_TYPEDEF_SHARED_PTR(PipelineCache)
    MAGMA_TYPEDEF_SHARED_PTR(PipelineLayout)
    MAGMA_TYPEDEF_SHARED_PTR(PipelineStatisticsQuery)
    MAGMA_TYPEDEF_SHARED_PTR(PrimaryCommandBuffer)
    MAGMA_TYPEDEF_SHARED_PTR(QueryPool)
    MAGMA_TYPEDEF_SHARED_PTR(Queue)
    MAGMA_TYPEDEF_SHARED_PTR(RenderPass)
    MAGMA_TYPEDEF_SHARED_PTR(Resource)
    MAGMA_TYPEDEF_SHARED_PTR(Sampler)
    MAGMA_TYPEDEF_SHARED_PTR(SecondaryCommandBuffer)
    MAGMA_TYPEDEF_SHARED_PTR(Semaphore)
    MAGMA_TYPEDEF_SHARED_PTR(ShaderModule)
    MAGMA_TYPEDEF_SHARED_PTR(SrcTransferBuffer)
    MAGMA_TYPEDEF_SHARED_PTR(StorageBuffer)
    MAGMA_TYPEDEF_SHARED_PTR(StorageImage2D);
    MAGMA_TYPEDEF_SHARED_PTR(StorageTexelBuffer)
    MAGMA_TYPEDEF_SHARED_PTR(SwapchainColorAttachment)
    MAGMA_TYPEDEF_SHARED_PTR(TimestampQuery)
    MAGMA_TYPEDEF_SHARED_PTR(UniformTexelBuffer)
    MAGMA_TYPEDEF_SHARED_PTR(UnnormalizedSampler)
    MAGMA_TYPEDEF_SHARED_PTR(VertexBuffer)

#ifdef VK_EXT_conditional_rendering
    MAGMA_TYPEDEF_SHARED_PTR(ConditionalRenderingBuffer)
#endif
#ifdef VK_EXT_debug_report
    MAGMA_TYPEDEF_SHARED_PTR(DebugReportCallback)
#endif
#ifdef VK_EXT_debug_utils
    MAGMA_TYPEDEF_SHARED_PTR(DebugUtilsMessenger)
#endif
#ifdef VK_EXT_headless_surface
    MAGMA_TYPEDEF_SHARED_PTR(HeadlessSurface)
#endif
#ifdef VK_EXT_full_screen_exclusive
    MAGMA_TYPEDEF_SHARED_PTR(FullScreenExclusiveSwapchain)
    MAGMA_TYPEDEF_SHARED_PTR(FullScreenExclusiveSwapchainWin32)
#endif
#ifdef VK_EXT_transform_feedback
    MAGMA_TYPEDEF_SHARED_PTR(TransformFeedbackBuffer)
    MAGMA_TYPEDEF_SHARED_PTR(TransformFeedbackCounterBuffer)
    MAGMA_TYPEDEF_SHARED_PTR(TransformFeedbackStreamQuery)
#endif
#ifdef VK_EXT_validation_cache
    MAGMA_TYPEDEF_SHARED_PTR(ValidationCache)
#endif

#ifdef VK_KHR_deferred_host_operations
    MAGMA_TYPEDEF_SHARED_PTR(DeferredOperation)
#endif
#ifdef VK_KHR_device_group
    MAGMA_TYPEDEF_SHARED_PTR(PhysicalDeviceGroup)
#endif
#ifdef VK_KHR_display
    MAGMA_TYPEDEF_SHARED_PTR(DisplayMode)
    MAGMA_TYPEDEF_SHARED_PTR(DisplaySurface)
#endif
#ifdef VK_KHR_imageless_framebuffer
    MAGMA_TYPEDEF_SHARED_PTR(ImagelessFramebuffer)
#endif
#ifdef VK_KHR_multiview
    MAGMA_TYPEDEF_SHARED_PTR(MultiviewRenderPass)
#endif
#ifdef VK_KHR_pipeline_executable_properties
    MAGMA_TYPEDEF_SHARED_PTR(PipelineExecutable)
#endif
#ifdef VK_KHR_sampler_ycbcr_conversion
    MAGMA_TYPEDEF_SHARED_PTR(SamplerYcbcrConversion)
#endif
#ifdef VK_KHR_surface
    MAGMA_TYPEDEF_SHARED_PTR(Surface)
#ifdef VK_KHR_win32_surface
    MAGMA_TYPEDEF_SHARED_PTR(Win32Surface)
#endif
#ifdef VK_KHR_xlib_surface
    MAGMA_TYPEDEF_SHARED_PTR(XlibSurface)
#endif
#ifdef VK_KHR_xcb_surface
    MAGMA_TYPEDEF_SHARED_PTR(XcbSurface)
#endif
#ifdef VK_KHR_wayland_surface
    MAGMA_TYPEDEF_SHARED_PTR(WaylandSurface)
#endif
#ifdef VK_QNX_screen_surface
    MAGMA_TYPEDEF_SHARED_PTR(QnxSurface)
#endif
#ifdef VK_KHR_android_surface
    MAGMA_TYPEDEF_SHARED_PTR(AndroidSurface)
#endif
#ifdef VK_MVK_ios_surface
    MAGMA_TYPEDEF_SHARED_PTR(iOSSurface)
#endif
#ifdef VK_MVK_macos_surface
    MAGMA_TYPEDEF_SHARED_PTR(MacOSSurface)
#endif
#ifdef VK_EXT_metal_surface
    MAGMA_TYPEDEF_SHARED_PTR(MetalSurface)
#endif
#ifdef VK_NN_vi_surface
    MAGMA_TYPEDEF_SHARED_PTR(ViSurface)
#endif
#endif // VK_KHR_surface
#ifdef VK_KHR_swapchain
    MAGMA_TYPEDEF_SHARED_PTR(Swapchain)
#endif
#ifdef VK_NV_ray_tracing
    MAGMA_TYPEDEF_SHARED_PTR(AccelerationStructure)
    MAGMA_TYPEDEF_SHARED_PTR(AccelerationStructureCompactedSizeQuery)
    MAGMA_TYPEDEF_SHARED_PTR(AccelerationStructureIndexBuffer)
    MAGMA_TYPEDEF_SHARED_PTR(AccelerationStructureInstanceBuffer)
    MAGMA_TYPEDEF_SHARED_PTR(AccelerationStructureVertexBuffer)
    MAGMA_TYPEDEF_SHARED_PTR(BottomLevelAccelerationStructure)
    MAGMA_TYPEDEF_SHARED_PTR(RayTracingBuffer)
    MAGMA_TYPEDEF_SHARED_PTR(RayTracingPipeline)
    MAGMA_TYPEDEF_SHARED_PTR(RayTracingPipelines)
    MAGMA_TYPEDEF_SHARED_PTR(TopLevelAccelerationStructure)
#endif // VK_NV_ray_tracing

    MAGMA_TYPEDEF_INTERFACE_SHARED_PTR(Allocator)
    MAGMA_TYPEDEF_INTERFACE_SHARED_PTR(DeviceMemoryAllocator)

    // Declare explicitly because of Xlib's Diplay
#ifdef VK_KHR_display
    class Display;
    typedef std::shared_ptr<Display> DisplayPtr;
#endif

    typedef std::function<void *(void *, const void *, std::size_t)> CopyMemoryFunction;
    typedef std::function<void *(void *, std::size_t)> ZeroMemoryFunction;
} // namespace magma
