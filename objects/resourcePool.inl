namespace magma
{
#ifdef MAGMA_X64
#define MAGMA_RESOURCE_POOL_IMPLEMENT_ACCESSOR(Type, pool)\
template<>\
inline core::Pool<Type>& ResourcePool::getPool<Type>()\
{\
    return pool;\
}

MAGMA_RESOURCE_POOL_IMPLEMENT_ACCESSOR(NonDispatchable<VkSemaphore>, semaphores)
MAGMA_RESOURCE_POOL_IMPLEMENT_ACCESSOR(NonDispatchable<VkFence>, fences)
MAGMA_RESOURCE_POOL_IMPLEMENT_ACCESSOR(NonDispatchable<VkDeviceMemory>, deviceMemories)
MAGMA_RESOURCE_POOL_IMPLEMENT_ACCESSOR(NonDispatchable<VkBuffer>, buffers)
MAGMA_RESOURCE_POOL_IMPLEMENT_ACCESSOR(NonDispatchable<VkImage>, images)
MAGMA_RESOURCE_POOL_IMPLEMENT_ACCESSOR(NonDispatchable<VkEvent>, events)
MAGMA_RESOURCE_POOL_IMPLEMENT_ACCESSOR(NonDispatchable<VkQueryPool>, queryPools)
MAGMA_RESOURCE_POOL_IMPLEMENT_ACCESSOR(NonDispatchable<VkBufferView>, bufferViews)
MAGMA_RESOURCE_POOL_IMPLEMENT_ACCESSOR(NonDispatchable<VkImageView>, imageViews)
MAGMA_RESOURCE_POOL_IMPLEMENT_ACCESSOR(NonDispatchable<VkShaderModule>, shaderModules)
MAGMA_RESOURCE_POOL_IMPLEMENT_ACCESSOR(NonDispatchable<VkPipelineCache>, pipelineCaches)
MAGMA_RESOURCE_POOL_IMPLEMENT_ACCESSOR(NonDispatchable<VkPipelineLayout>, pipelineLayouts)
MAGMA_RESOURCE_POOL_IMPLEMENT_ACCESSOR(NonDispatchable<VkRenderPass>, renderPasses)
MAGMA_RESOURCE_POOL_IMPLEMENT_ACCESSOR(NonDispatchable<VkPipeline>, pipelines)
MAGMA_RESOURCE_POOL_IMPLEMENT_ACCESSOR(NonDispatchable<VkDescriptorSetLayout>, descriptorSetLayouts)
MAGMA_RESOURCE_POOL_IMPLEMENT_ACCESSOR(NonDispatchable<VkSampler>, samplers)
MAGMA_RESOURCE_POOL_IMPLEMENT_ACCESSOR(NonDispatchable<VkDescriptorPool>, descriptorPools)
MAGMA_RESOURCE_POOL_IMPLEMENT_ACCESSOR(NonDispatchable<VkDescriptorSet>, descriptorSets)
MAGMA_RESOURCE_POOL_IMPLEMENT_ACCESSOR(NonDispatchable<VkFramebuffer>, framebuffers)
MAGMA_RESOURCE_POOL_IMPLEMENT_ACCESSOR(NonDispatchable<VkCommandPool>, commandPools)
#ifdef VK_KHR_surface
MAGMA_RESOURCE_POOL_IMPLEMENT_ACCESSOR(NonDispatchable<VkSurfaceKHR>, surfaces)
#endif
MAGMA_RESOURCE_POOL_IMPLEMENT_ACCESSOR(NonDispatchable<VkSwapchainKHR>, swapchains)
#ifdef VK_KHR_display
MAGMA_RESOURCE_POOL_IMPLEMENT_ACCESSOR(NonDispatchable<VkDisplayKHR>, displays)
MAGMA_RESOURCE_POOL_IMPLEMENT_ACCESSOR(NonDispatchable<VkDisplayModeKHR>, displayModes)
#endif
#ifdef VK_EXT_debug_report
MAGMA_RESOURCE_POOL_IMPLEMENT_ACCESSOR(NonDispatchable<VkDebugReportCallbackEXT>, debugReportCallbacks)
#endif
#ifdef VK_EXT_debug_utils
MAGMA_RESOURCE_POOL_IMPLEMENT_ACCESSOR(NonDispatchable<VkDebugUtilsMessengerEXT>, debugUtilsMessengers)
#endif
#ifdef VK_NV_ray_tracing
MAGMA_RESOURCE_POOL_IMPLEMENT_ACCESSOR(NonDispatchable<VkAccelerationStructureNV>, accelerationStructures)
#endif
#endif // MAGMA_X64
} // namespace magma
