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
    MAGMA_TYPEDEF_MANAGED_PTR(Buffer)
    MAGMA_TYPEDEF_MANAGED_PTR(BufferView)
    MAGMA_TYPEDEF_MANAGED_PTR(ColorAttachment)
    MAGMA_TYPEDEF_MANAGED_PTR(CommandBuffer)
    MAGMA_TYPEDEF_MANAGED_PTR(CommandPool)
    MAGMA_TYPEDEF_MANAGED_PTR(ComputePipeline)
    MAGMA_TYPEDEF_MANAGED_PTR(ComputePipelineBatch)
    MAGMA_TYPEDEF_MANAGED_PTR(CountBuffer)
    MAGMA_TYPEDEF_MANAGED_PTR(DeferredOperation)
    MAGMA_TYPEDEF_MANAGED_PTR(DepthStencilAttachment)
    MAGMA_TYPEDEF_MANAGED_PTR(DescriptorPool)
    MAGMA_TYPEDEF_MANAGED_PTR(DescriptorSet)
    MAGMA_TYPEDEF_MANAGED_PTR(DescriptorSetLayout)
    MAGMA_TYPEDEF_MANAGED_PTR(Device)
    MAGMA_TYPEDEF_MANAGED_PTR(DeviceChild)
    MAGMA_TYPEDEF_MANAGED_PTR(DeviceMemory)
    MAGMA_TYPEDEF_MANAGED_PTR(DispatchCountBuffer)
    MAGMA_TYPEDEF_MANAGED_PTR(DispatchIndirectBuffer)
    MAGMA_TYPEDEF_MANAGED_PTR(DrawIndexedIndirectBuffer)
    MAGMA_TYPEDEF_MANAGED_PTR(DrawIndirectBuffer)
    MAGMA_TYPEDEF_MANAGED_PTR(DstTransferBuffer)
    MAGMA_TYPEDEF_MANAGED_PTR(DynamicIndexBuffer)
    MAGMA_TYPEDEF_MANAGED_PTR(DynamicStorageBuffer)
    MAGMA_TYPEDEF_MANAGED_PTR(DynamicStorageTexelBuffer)
    MAGMA_TYPEDEF_MANAGED_PTR(DynamicVertexBuffer)
    MAGMA_TYPEDEF_MANAGED_PTR(Event)
    MAGMA_TYPEDEF_MANAGED_PTR(ExternalImage2D)
    MAGMA_TYPEDEF_MANAGED_PTR(Fence)
    MAGMA_TYPEDEF_MANAGED_PTR(Framebuffer)
    MAGMA_TYPEDEF_MANAGED_PTR(GraphicsPipeline)
    MAGMA_TYPEDEF_MANAGED_PTR(GraphicsPipelineBatch)
    MAGMA_TYPEDEF_MANAGED_PTR(HostStorageBuffer)
    MAGMA_TYPEDEF_MANAGED_PTR(Image)
    MAGMA_TYPEDEF_MANAGED_PTR(Image1D)
    MAGMA_TYPEDEF_MANAGED_PTR(Image1DArray)
    MAGMA_TYPEDEF_MANAGED_PTR(Image2D)
    MAGMA_TYPEDEF_MANAGED_PTR(Image2DArray)
    MAGMA_TYPEDEF_MANAGED_PTR(Image3D)
    MAGMA_TYPEDEF_MANAGED_PTR(ImageCube)
    MAGMA_TYPEDEF_MANAGED_PTR(ImageView)
    MAGMA_TYPEDEF_MANAGED_PTR(IndexBuffer)
    MAGMA_TYPEDEF_MANAGED_PTR(IndirectBuffer)
    MAGMA_TYPEDEF_MANAGED_PTR(Instance)
    MAGMA_TYPEDEF_MANAGED_PTR(LinearTiledImage2D)
    MAGMA_TYPEDEF_MANAGED_PTR(LodSampler)
    MAGMA_TYPEDEF_MANAGED_PTR(MutableImage)
    MAGMA_TYPEDEF_MANAGED_PTR(MutableImage2D)
    MAGMA_TYPEDEF_MANAGED_PTR(MutableImageView)
    MAGMA_TYPEDEF_MANAGED_PTR(OcclusionQuery)
    MAGMA_TYPEDEF_MANAGED_PTR(PhysicalDevice)
    MAGMA_TYPEDEF_MANAGED_PTR(Pipeline)
    MAGMA_TYPEDEF_MANAGED_PTR(PipelineCache)
    MAGMA_TYPEDEF_MANAGED_PTR(PipelineLayout)
    MAGMA_TYPEDEF_MANAGED_PTR(PipelineStatisticsQuery)
    MAGMA_TYPEDEF_MANAGED_PTR(PrimaryCommandBuffer)
    MAGMA_TYPEDEF_MANAGED_PTR(QueryPool)
    MAGMA_TYPEDEF_MANAGED_PTR(Queue)
    MAGMA_TYPEDEF_MANAGED_PTR(RenderPass)
    MAGMA_TYPEDEF_MANAGED_PTR(Resource)
    MAGMA_TYPEDEF_MANAGED_PTR(Sampler)
    MAGMA_TYPEDEF_MANAGED_PTR(SecondaryCommandBuffer)
    MAGMA_TYPEDEF_MANAGED_PTR(Semaphore)
    MAGMA_TYPEDEF_MANAGED_PTR(ShaderModule)
    MAGMA_TYPEDEF_MANAGED_PTR(SrcTransferBuffer)
    MAGMA_TYPEDEF_MANAGED_PTR(StorageBuffer)
    MAGMA_TYPEDEF_MANAGED_PTR(StorageImage2D)
    MAGMA_TYPEDEF_MANAGED_PTR(StorageTexelBuffer)
    MAGMA_TYPEDEF_MANAGED_PTR(TimestampQuery)
    MAGMA_TYPEDEF_MANAGED_PTR(UniformTexelBuffer)
    MAGMA_TYPEDEF_MANAGED_PTR(UnnormalizedSampler)
    MAGMA_TYPEDEF_MANAGED_PTR(VertexBuffer)

#ifdef VK_EXT_attachment_feedback_loop_layout
    MAGMA_TYPEDEF_MANAGED_PTR(FeedbackColorAttachment)
    MAGMA_TYPEDEF_MANAGED_PTR(FeedbackDepthStencilAttachment)
#endif
#ifdef VK_EXT_conditional_rendering
    MAGMA_TYPEDEF_MANAGED_PTR(ConditionalRenderingBuffer)
#endif
#ifdef VK_EXT_debug_report
    MAGMA_TYPEDEF_MANAGED_PTR(DebugReportCallback)
#endif
#ifdef VK_EXT_debug_utils
    MAGMA_TYPEDEF_MANAGED_PTR(DebugUtilsMessenger)
#endif
#ifdef VK_EXT_descriptor_indexing
    MAGMA_TYPEDEF_MANAGED_PTR(VariableCountDescriptorSet)
#endif
#ifdef VK_EXT_fragment_density_map
    MAGMA_TYPEDEF_MANAGED_PTR(FragmentDensityMap)
    MAGMA_TYPEDEF_MANAGED_PTR(FragmentDensityMapView)
#endif
#ifdef VK_EXT_full_screen_exclusive
    MAGMA_TYPEDEF_MANAGED_PTR(FullScreenExclusiveSwapchain)
#endif
#ifdef VK_EXT_graphics_pipeline_library
    MAGMA_TYPEDEF_MANAGED_PTR(GraphicsPipelineLibrary)
#endif
#ifdef VK_EXT_headless_surface
    MAGMA_TYPEDEF_MANAGED_PTR(HeadlessSurface)
#endif
#if defined(VK_EXT_mesh_shader) || defined(VK_NV_mesh_shader)
    MAGMA_TYPEDEF_MANAGED_PTR(DrawMeshTasksIndirectBuffer)
    MAGMA_TYPEDEF_MANAGED_PTR(MeshPrimitivesQuery)
#endif
#ifdef VK_EXT_private_data
    MAGMA_TYPEDEF_MANAGED_PTR(PrivateDataSlot)
#endif
#ifdef VK_EXT_transform_feedback
    MAGMA_TYPEDEF_MANAGED_PTR(TransformFeedbackBuffer)
    MAGMA_TYPEDEF_MANAGED_PTR(TransformFeedbackCounterBuffer)
    MAGMA_TYPEDEF_MANAGED_PTR(TransformFeedbackQuery)
#endif
#ifdef VK_EXT_validation_cache
    MAGMA_TYPEDEF_MANAGED_PTR(ValidationCache)
#endif
#ifdef VK_KHR_acceleration_structure
    MAGMA_TYPEDEF_MANAGED_PTR(AccelerationStructure)
    MAGMA_TYPEDEF_MANAGED_PTR(AccelerationStructureInputBuffer)
    MAGMA_TYPEDEF_MANAGED_PTR(AccelerationStructureQuery)
    MAGMA_TYPEDEF_MANAGED_PTR(AccelerationStructureStorageBuffer)
    MAGMA_TYPEDEF_MANAGED_PTR(BottomLevelAccelerationStructure)
    MAGMA_TYPEDEF_MANAGED_PTR(GenericAccelerationStructure)
    MAGMA_TYPEDEF_MANAGED_PTR(TopLevelAccelerationStructure)
#endif
#ifdef VK_KHR_deferred_host_operations
    MAGMA_TYPEDEF_MANAGED_PTR(DeferredOperation)
#endif
#ifdef VK_KHR_device_group
    MAGMA_TYPEDEF_MANAGED_PTR(PhysicalDeviceGroup)
#endif
#ifdef VK_KHR_display
    class Display; // Declare explicitly because of Xlib's Diplay
    MAGMA_TYPEDEF_MANAGED_PTR(Display)
    MAGMA_TYPEDEF_MANAGED_PTR(DisplayMode)
    MAGMA_TYPEDEF_MANAGED_PTR(DisplaySurface)
#endif
#ifdef VK_KHR_external_fence
    MAGMA_TYPEDEF_MANAGED_PTR(ExternalFence)
#endif
#ifdef VK_KHR_external_semaphore
    MAGMA_TYPEDEF_MANAGED_PTR(ExternalSemaphore)
#endif
#ifdef VK_KHR_fragment_shading_rate
    MAGMA_TYPEDEF_MANAGED_PTR(FragmentShadingRateAttachment)
#endif
#ifdef VK_KHR_imageless_framebuffer
    MAGMA_TYPEDEF_MANAGED_PTR(ImagelessFramebuffer)
#endif
#ifdef VK_KHR_maintenance1
    MAGMA_TYPEDEF_MANAGED_PTR(VolumeAttachment)
#endif
#ifdef VK_KHR_multiview
    MAGMA_TYPEDEF_MANAGED_PTR(MultiviewRenderPass)
#endif
#ifdef VK_KHR_performance_query
    MAGMA_TYPEDEF_MANAGED_PTR(PerformanceQuery)
#endif
#ifdef VK_KHR_pipeline_executable_properties
    MAGMA_TYPEDEF_MANAGED_PTR(PipelineExecutable)
#endif
#ifdef VK_KHR_pipeline_library
    MAGMA_TYPEDEF_MANAGED_PTR(PipelineLibrary)
    MAGMA_TYPEDEF_MANAGED_PTR(ComputePipelineLibrary)
#endif
#ifdef VK_KHR_ray_tracing_pipeline
    MAGMA_TYPEDEF_MANAGED_PTR(RayTracingPipeline)
    MAGMA_TYPEDEF_MANAGED_PTR(RayTracingPipelineBatch)
    MAGMA_TYPEDEF_MANAGED_PTR(ShaderBindingTable)
#endif
#ifdef VK_KHR_sampler_ycbcr_conversion
    MAGMA_TYPEDEF_MANAGED_PTR(SamplerYcbcrConversion)
#endif
#ifdef VK_KHR_timeline_semaphore
    MAGMA_TYPEDEF_MANAGED_PTR(TimelineSemaphore)
    #if defined(VK_KHR_external_semaphore) && defined(VK_KHR_external_semaphore_win32)
    MAGMA_TYPEDEF_MANAGED_PTR(D3d12ExternalTimelineSemaphore)
    #endif
#endif
#ifdef VK_KHR_surface
    MAGMA_TYPEDEF_MANAGED_PTR(Surface)
#endif
#ifdef VK_KHR_swapchain
    MAGMA_TYPEDEF_MANAGED_PTR(Swapchain)
    MAGMA_TYPEDEF_MANAGED_PTR(SwapchainImage)
#endif

    MAGMA_TYPEDEF_MANAGED_PTR(IDeviceMemory)
    MAGMA_TYPEDEF_MANAGED_PTR(IObject)
    MAGMA_TYPEDEF_MANAGED_PTR(IResource)
} // namespace magma
