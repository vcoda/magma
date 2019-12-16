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
    class Display; // Declare explicitly because of Xlib's Diplay

    typedef std::shared_ptr<class IAllocator> AllocatorPtr;

    typedef std::shared_ptr<class Buffer> BufferPtr;
    typedef std::shared_ptr<class BufferView> BufferViewPtr;
    typedef std::shared_ptr<class CommandBuffer> CommandBufferPtr;
    typedef std::shared_ptr<class CommandPool> CommandPoolPtr;
    typedef std::shared_ptr<class CompactedSizeQuery> CompactedSizeQueryPtr;
    typedef std::shared_ptr<class ComputePipeline> ComputePipelinePtr;
    typedef std::shared_ptr<class ConditionalRenderingBuffer> ConditionalRenderingBufferPtr;
    typedef std::shared_ptr<class DebugReportCallback> DebugReportCallbackPtr;
    typedef std::shared_ptr<class DebugUtilsMessenger> DebugUtilsMessengerPtr;
    typedef std::shared_ptr<class DescriptorPool> DescriptorPoolPtr;
    typedef std::shared_ptr<class DescriptorSet> DescriptorSetPtr;
    typedef std::shared_ptr<class DescriptorSetLayout> DescriptorSetLayoutPtr;
    typedef std::shared_ptr<class Device> DevicePtr;
    typedef std::shared_ptr<class DeviceMemory> DeviceMemoryPtr;
    typedef std::shared_ptr<Display> DisplayPtr;
    typedef std::shared_ptr<class DisplayMode> DisplayModePtr;
    typedef std::shared_ptr<class DisplaySurface> DisplaySurfacePtr;
    typedef std::shared_ptr<class DstTransferBuffer> DstTransferBufferPtr;
    typedef std::shared_ptr<class Event> EventPtr;
    typedef std::shared_ptr<class Fence> FencePtr;
    typedef std::shared_ptr<class Framebuffer> FramebufferPtr;
    typedef std::shared_ptr<class GraphicsPipeline> GraphicsPipelinePtr;
    typedef std::shared_ptr<class Image> ImagePtr;
    typedef std::shared_ptr<class Image1D> Image1DPtr;
    typedef std::shared_ptr<class Image1DArray> Image1DArrayPtr;
    typedef std::shared_ptr<class Image2D> Image2DPtr;
    typedef std::shared_ptr<class Image2DArray> Image2DArrayPtr;
    typedef std::shared_ptr<class Image3D> Image3DPtr;
    typedef std::shared_ptr<class ImageCube> ImageCubePtr;
    typedef std::shared_ptr<class ImageView> ImageViewPtr;
    typedef std::shared_ptr<class IndexBuffer> IndexBufferPtr;
    typedef std::shared_ptr<class IndirectBuffer> IndirectBufferPtr;
    typedef std::shared_ptr<class Instance> InstancePtr;
    typedef std::shared_ptr<class OcclusionQuery> OcclusionQueryPtr;
    typedef std::shared_ptr<class Object> ObjectPtr;
    typedef std::shared_ptr<class PhysicalDevice> PhysicalDevicePtr;
    typedef std::shared_ptr<class PhysicalDeviceGroup> PhysicalDeviceGroupPtr;
    typedef std::shared_ptr<class Pipeline> PipelinePtr;
    typedef std::shared_ptr<class PipelineCache> PipelineCachePtr;
    typedef std::shared_ptr<class PipelineLayout> PipelineLayoutPtr;
    typedef std::shared_ptr<class PipelineStatisticsQuery> PipelineStatisticsQueryPtr;
    typedef std::shared_ptr<class RayTracingBuffer> RayTracingBufferPtr;
    typedef std::shared_ptr<class RayTracingPipeline> RayTracingPipelinePtr;
    typedef std::shared_ptr<class QueryPool> QueryPoolPtr;
    typedef std::shared_ptr<class Queue> QueuePtr;
    typedef std::shared_ptr<class RenderPass> RenderPassPtr;
    typedef std::shared_ptr<class Sampler> SamplerPtr;
    typedef std::shared_ptr<class Semaphore> SemaphorePtr;
    typedef std::shared_ptr<class ShaderModule> ShaderModulePtr;
    typedef std::shared_ptr<class SrcTransferBuffer> SrcTransferBufferPtr;
    typedef std::shared_ptr<class StorageBuffer> StorageBufferPtr;
    typedef std::shared_ptr<class StorageTexelBuffer> StorageTexelBufferPtr;
    typedef std::shared_ptr<class Surface> SurfacePtr;
    typedef std::shared_ptr<class Swapchain> SwapchainPtr;
    typedef std::shared_ptr<class TimestampQuery> TimestampQueryPtr;
    typedef std::shared_ptr<class UniformTexelBuffer> UniformTexelBufferPtr;
    typedef std::shared_ptr<class ValidationCache> ValidationCachePtr;
    typedef std::shared_ptr<class VertexBuffer> VertexBufferPtr;

    typedef std::shared_ptr<class Win32Surface> Win32SurfacePtr;
    typedef std::shared_ptr<class XlibSurface> XlibSurfacePtr;
    typedef std::shared_ptr<class XcbSurface> XcbSurfacePtr;
    typedef std::shared_ptr<class WaylandSurface> WaylandSurfacePtr;
    typedef std::shared_ptr<class MirSurface> MirSurfacePtr;
    typedef std::shared_ptr<class AndroidSurface> AndroidSurfacePtr;
    typedef std::shared_ptr<class iOSSurface> iOSSurfacePtr;
    typedef std::shared_ptr<class MacOSSurface> MacOSSurfacePtr;
    typedef std::shared_ptr<class MetalSurface> MetalSurfacePtr;

    typedef std::unique_ptr<class InstanceExtensions> InstanceExtensionsPtr;
    typedef std::unique_ptr<class PhysicalDeviceExtensions> PhysicalDeviceExtensionsPtr;

    namespace aux
    {
        typedef std::shared_ptr<class BaseFramebuffer> BaseFramebufferPtr;
        typedef std::shared_ptr<class BlitRectangle> BlitRectanglePtr;
        typedef std::shared_ptr<class Framebuffer> FramebufferPtr;
        typedef std::shared_ptr<class GraphicsPipelineCache> GraphicsPipelineCachePtr;
        typedef std::shared_ptr<class ImmediateRender> ImmediateRenderPtr;
        typedef std::shared_ptr<class IShaderIncludeHandler> ShaderIncludeHandlerPtr;
        typedef std::shared_ptr<class MipmapGenerator> MipmapGeneratorPtr;
        typedef std::shared_ptr<class MultisampleFramebuffer> MultisampleFramebufferPtr;
        typedef std::shared_ptr<class ShaderCompiler> ShaderCompilerPtr;
        typedef std::shared_ptr<class SwapchainFramebuffer> SwapchainFramebufferPtr;
    } // namespace aux

    typedef std::function<void *(void *, const void *, std::size_t)> CopyMemoryFunction;
    typedef std::function<void *(void *, std::size_t)> ZeroMemoryFunction;
} // namespace magma
