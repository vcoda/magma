#define MAGMA_RESOURCE_POOL(Type, pool)\
template<>\
inline magma::Pool<Type>& magma::DeviceResourcePool::getPool<Type>()\
{\
    return pool;\
}

MAGMA_RESOURCE_POOL(VkSemaphore, semaphores)
MAGMA_RESOURCE_POOL(VkFence, fences)
MAGMA_RESOURCE_POOL(VkDeviceMemory, deviceMemories)
MAGMA_RESOURCE_POOL(VkBuffer, buffers)
MAGMA_RESOURCE_POOL(VkImage, images)
MAGMA_RESOURCE_POOL(VkEvent, events)
MAGMA_RESOURCE_POOL(VkQueryPool, queryPools)
MAGMA_RESOURCE_POOL(VkBufferView, bufferViews)
MAGMA_RESOURCE_POOL(VkImageView, imageViews)
MAGMA_RESOURCE_POOL(VkShaderModule, shaderModules)
MAGMA_RESOURCE_POOL(VkPipelineCache, pipelineCaches)
MAGMA_RESOURCE_POOL(VkPipelineLayout, pipelineLayouts)
MAGMA_RESOURCE_POOL(VkRenderPass, renderPasses)
MAGMA_RESOURCE_POOL(VkPipeline, pipelines)
MAGMA_RESOURCE_POOL(VkDescriptorSetLayout, descriptorSetLayouts)
MAGMA_RESOURCE_POOL(VkSampler, samplers)
MAGMA_RESOURCE_POOL(VkDescriptorPool, descriptorPools)
MAGMA_RESOURCE_POOL(VkDescriptorSet, descriptorSets)
MAGMA_RESOURCE_POOL(VkFramebuffer, framebuffers)
MAGMA_RESOURCE_POOL(VkCommandPool, commandPools)
#ifdef VK_KHR_deferred_host_operations
MAGMA_RESOURCE_POOL(VkDeferredOperationKHR, deferredOperations)
#endif
#ifdef VK_KHR_sampler_ycbcr_conversion
MAGMA_RESOURCE_POOL(VkSamplerYcbcrConversionKHR, ycbcrSamplers)
#endif
#ifdef VK_KHR_surface
MAGMA_RESOURCE_POOL(VkSurfaceKHR, surfaces)
#endif
MAGMA_RESOURCE_POOL(VkSwapchainKHR, swapchains)
#ifdef VK_KHR_display
MAGMA_RESOURCE_POOL(VkDisplayKHR, displays)
MAGMA_RESOURCE_POOL(VkDisplayModeKHR, displayModes)
#endif
#ifdef VK_EXT_debug_report
MAGMA_RESOURCE_POOL(VkDebugReportCallbackEXT, debugReportCallbacks)
#endif
#ifdef VK_EXT_debug_utils
MAGMA_RESOURCE_POOL(VkDebugUtilsMessengerEXT, debugUtilsMessengers)
#endif
#ifdef VK_EXT_private_data
MAGMA_RESOURCE_POOL(VkPrivateDataSlotEXT, privateDataSlots)
#endif
#ifdef VK_NV_ray_tracing
MAGMA_RESOURCE_POOL(VkAccelerationStructureNV, accelerationStructures)
#endif

namespace magma
{
template<class NonDispatchableChild, class NonDispatchable>
inline void DeviceResourcePool::foreach(const Pool<NonDispatchable>& pool, const Fn<NonDispatchableChild>& fn) const
{
    std::lock_guard<std::mutex> guard(mtx);
    std::for_each(pool.begin(), pool.end(),
        [&fn](auto parent)
        {
            auto child = dynamic_cast<const NonDispatchableChild *>(parent);
            if (child)
                fn(child);
        });
}
} // namespace magma
