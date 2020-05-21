namespace magma
{
template<typename Base>
inline void ResourcePool::Pool<Base>::registerResource(const Base *resource) noexcept
{
    try {
        std::lock_guard<std::mutex> guard(mutex);
        resources.insert(resource);
    } catch (...) {
    }
}

template<typename Base>
inline void ResourcePool::Pool<Base>::unregisterResource(const Base *resource) noexcept
{
    try {
        std::lock_guard<std::mutex> guard(mutex);
        auto it = resources.find(resource);
        MAGMA_ASSERT(it != resources.end());
        if (it != resources.end())
            resources.erase(it);
    } catch (...) {
    }
}

template<typename Base>
inline uint32_t ResourcePool::Pool<Base>::count() const noexcept
{
    try {
        std::lock_guard<std::mutex> guard(mutex);
        return static_cast<uint32_t>(resources.size());
    } catch (...) {
        return 0;
    }
}

template<typename Base>
template<typename Derived>
void ResourcePool::Pool<Base>::forEach(const std::function<void(const Derived *resource)>& fn) const noexcept
{
    try {
        std::lock_guard<std::mutex> guard(mutex);
        std::for_each(resources.begin(), resources.end(),
            [&fn](const Base *base)
            {
                const Derived *derived = dynamic_cast<const Derived *>(base);
                if (derived)
                    fn(derived);
            });
    } catch (...) {
    }
}

#ifdef MAGMA_X64
#define MAGMA_RESOURCE_POOL_IMPLEMENT_ACCESSOR(Type, pool)\
template<>\
inline ResourcePool::Pool<Type>& ResourcePool::getPool<Type>()\
{\
    std::lock_guard<std::mutex> guard(mutex);\
    return pool;\
}

// Core types
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
// Non-core types
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
