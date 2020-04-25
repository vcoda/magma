#if !defined(MAGMA_DEBUG)
#ifdef VK_EXT_debug_utils
#undef VK_EXT_debug_utils
#endif
#ifdef VK_EXT_debug_marker
#undef VK_EXT_debug_marker
#endif
#endif // !MAGMA_DEBUG

namespace magma
{
template<typename Type>
inline Object<Type>::Object(VkObjectType objectType, std::shared_ptr<Device> device, std::shared_ptr<IAllocator> allocator) noexcept:
#if !defined(MAGMA_X64)
    objectType(objectType),
#endif
    device(std::move(device)),
    allocator(std::move(allocator))
{
    MAGMA_UNUSED(objectType);
}

template<typename Type>
inline void Object<Type>::setObjectName(const char *name) noexcept
{
    MAGMA_ASSERT(name);
    MAGMA_ASSERT(strlen(name) > 0);
#ifdef VK_EXT_debug_utils
    if (device)
    {
        MAGMA_OPTIONAL_DEVICE_EXTENSION(vkSetDebugUtilsObjectNameEXT);
        if (vkSetDebugUtilsObjectNameEXT)
        {
            VkDebugUtilsObjectNameInfoEXT info;
            info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
            info.pNext = nullptr;
            info.objectType = getObjectType();
            info.objectHandle = getHandle();
            info.pObjectName = name;
            vkSetDebugUtilsObjectNameEXT(MAGMA_HANDLE(device), &info);
        }
    }
#elif defined(VK_EXT_debug_marker)
    if (device)
    {
        MAGMA_OPTIONAL_DEVICE_EXTENSION(vkDebugMarkerSetObjectNameEXT);
        if (vkDebugMarkerSetObjectNameEXT)
        {
            VkDebugMarkerObjectNameInfoEXT info;
            info.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_NAME_INFO_EXT;
            info.pNext = nullptr;
            info.objectType = helpers::castToDebugReportType(getObjectType());
            info.object = getHandle();
            info.pObjectName = name;
            vkDebugMarkerSetObjectNameEXT(MAGMA_HANDLE(device), &info);
        }
    }
#else
    MAGMA_UNUSED(name);
#endif // VK_EXT_debug_marker
}

template<typename Type>
inline void Object<Type>::setObjectTag(uint64_t tagName, std::size_t tagSize, const void *tag) noexcept
{
    MAGMA_ASSERT(tagName);
    MAGMA_ASSERT(tagSize);
    MAGMA_ASSERT(tag);
#ifdef VK_EXT_debug_utils
    if (device)
    {
        MAGMA_OPTIONAL_DEVICE_EXTENSION(vkSetDebugUtilsObjectTagEXT);
        if(vkSetDebugUtilsObjectTagEXT)
        {
            VkDebugUtilsObjectTagInfoEXT info;
            info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_TAG_INFO_EXT;
            info.pNext = nullptr;
            info.objectType = getObjectType();
            info.objectHandle = getHandle();
            info.tagName = tagName;
            info.tagSize = tagSize;
            info.pTag = tag;
            vkSetDebugUtilsObjectTagEXT(MAGMA_HANDLE(device), &info);
        }
    }
#elif defined(VK_EXT_debug_marker)
    if (device)
    {
        MAGMA_OPTIONAL_DEVICE_EXTENSION(vkDebugMarkerSetObjectTagEXT);
        if (vkDebugMarkerSetObjectTagEXT)
        {
            VkDebugMarkerObjectTagInfoEXT info;
            info.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_TAG_INFO_EXT;
            info.pNext = nullptr;
            info.objectType = helpers::castToDebugReportType(getObjectType());
            info.object = getHandle();
            info.tagName = tagName;
            info.tagSize = tagSize;
            info.pTag = tag;
            vkDebugMarkerSetObjectTagEXT(MAGMA_HANDLE(device), &info);
        }
    }
#else
    MAGMA_UNUSED(tagName);
    MAGMA_UNUSED(tagSize);
    MAGMA_UNUSED(tag);
#endif // VK_EXT_debug_marker
}

template<typename Type>
template<typename TagType>
inline void Object<Type>::setObjectTag(uint64_t tagName, const TagType& tag) noexcept
{
    setObjectTag(tagName, sizeof(TagType), &tag);
}
} // namespace magma
