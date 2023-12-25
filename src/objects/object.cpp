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
#include "pch.h"
#pragma hdrstop
#include "object.h"
#include "device.h"
#include "privateDataSlot.h"
#include "../misc/extension.h"
#include "../helpers/castToDebugReport.h"
#include "../exceptions/errorResult.h"

namespace magma
{
std::mutex Object::mtx;

void Object::setPrivateData(uint64_t data)
{
    const uint64_t handle = getHandle();
#ifdef VK_EXT_private_data
    std::shared_ptr<PrivateDataSlot> privateDataSlot = device->getPrivateDataSlot();
    if (privateDataSlot)
    {
        MAGMA_REQUIRED_DEVICE_EXTENSION(vkSetPrivateDataEXT, VK_EXT_PRIVATE_DATA_EXTENSION_NAME);
        const VkResult result = vkSetPrivateDataEXT(MAGMA_HANDLE(device), getObjectType(), handle, *privateDataSlot, data);
        MAGMA_HANDLE_RESULT(result, "failed to set private data");
    }
#endif // VK_EXT_private_data
    // Fallback if extension not present
    std::lock_guard<std::mutex> lock(mtx);
    std::unordered_map<uint64_t, uint64_t>& privateData = device->getPrivateDataMap();
    privateData[handle] = data;
}
        
uint64_t Object::getPrivateData() const
{
    const uint64_t handle = getHandle();
#ifdef VK_EXT_private_data
    std::shared_ptr<PrivateDataSlot> privateDataSlot = device->getPrivateDataSlot();
    if (privateDataSlot)
    {
        MAGMA_REQUIRED_DEVICE_EXTENSION(vkGetPrivateDataEXT, VK_EXT_PRIVATE_DATA_EXTENSION_NAME);
        uint64_t data = 0;
        vkGetPrivateDataEXT(MAGMA_HANDLE(device), getObjectType(), handle, *privateDataSlot, &data);
        return data;
    }
#endif // VK_EXT_private_data
    // Fallback if extension not present
    std::lock_guard<std::mutex> lock(mtx);
    std::unordered_map<uint64_t, uint64_t>& privateData = device->getPrivateDataMap();
    auto it = privateData.find(handle);
    if (it != privateData.end())
        return it->second;
    return 0;
}

#ifdef MAGMA_DEBUG
void Object::setDebugName(const std::string& name_)
{
    MAGMA_ASSERT(!name_.empty());
    name = name_;
    VkResult result = VK_SUCCESS;
    if (device)
    {
    #ifdef VK_EXT_debug_utils
        MAGMA_DEVICE_EXTENSION(vkSetDebugUtilsObjectNameEXT);
        if (vkSetDebugUtilsObjectNameEXT)
        {
            VkDebugUtilsObjectNameInfoEXT objectNameInfo;
            objectNameInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
            objectNameInfo.pNext = nullptr;
            objectNameInfo.objectType = getObjectType();
            objectNameInfo.objectHandle = getHandle();
            objectNameInfo.pObjectName = name.c_str();
            result = vkSetDebugUtilsObjectNameEXT(MAGMA_HANDLE(device), &objectNameInfo);
        } else
        {   // VK_EXT_debug_marker had been deprecated, but let's try
            // to fallback to it in the case of old drivers/SDK.
    #endif // VK_EXT_debug_utils
        #ifdef VK_EXT_debug_marker
            MAGMA_DEVICE_EXTENSION(vkDebugMarkerSetObjectNameEXT);
            if (vkDebugMarkerSetObjectNameEXT)
            {
                VkDebugMarkerObjectNameInfoEXT objectNameInfo;
                objectNameInfo.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_NAME_INFO_EXT;
                objectNameInfo.pNext = nullptr;
                objectNameInfo.objectType = helpers::objectToDebugReportType(getObjectType());
                objectNameInfo.object = getHandle();
                objectNameInfo.pObjectName = name.c_str();
                result = vkDebugMarkerSetObjectNameEXT(MAGMA_HANDLE(device), &objectNameInfo);
            }
        #endif // VK_EXT_debug_marker
    #ifdef VK_EXT_debug_utils
        }
    #endif
    }
    MAGMA_HANDLE_RESULT(result, "failed to set object name");
}

void Object::setDebugTag(uint64_t tagName_, std::size_t tagSize, const void *tag)
{
    MAGMA_ASSERT(tagName_);
    MAGMA_ASSERT(tagSize > 0);
    MAGMA_ASSERT(tag);
    tagName = tagName_;
    VkResult result = VK_SUCCESS;
    if (device)
    {
    #ifdef VK_EXT_debug_utils
        MAGMA_DEVICE_EXTENSION(vkSetDebugUtilsObjectTagEXT);
        if (vkSetDebugUtilsObjectTagEXT)
        {
            VkDebugUtilsObjectTagInfoEXT objectTagInfo;
            objectTagInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_TAG_INFO_EXT;
            objectTagInfo.pNext = nullptr;
            objectTagInfo.objectType = getObjectType();
            objectTagInfo.objectHandle = getHandle();
            objectTagInfo.tagName = tagName;
            objectTagInfo.tagSize = tagSize;
            objectTagInfo.pTag = tag;
            result = vkSetDebugUtilsObjectTagEXT(MAGMA_HANDLE(device), &objectTagInfo);
        } else
        {   // VK_EXT_debug_marker had been deprecated, but let's try
            // to fallback to it in the case of old drivers/SDK.
    #endif // VK_EXT_debug_utils
        #ifdef VK_EXT_debug_marker
            MAGMA_DEVICE_EXTENSION(vkDebugMarkerSetObjectTagEXT);
            if (vkDebugMarkerSetObjectTagEXT)
            {
                VkDebugMarkerObjectTagInfoEXT objectTagInfo;
                objectTagInfo.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_TAG_INFO_EXT;
                objectTagInfo.pNext = nullptr;
                objectTagInfo.objectType = helpers::objectToDebugReportType(getObjectType());
                objectTagInfo.object = getHandle();
                objectTagInfo.tagName = tagName;
                objectTagInfo.tagSize = tagSize;
                objectTagInfo.pTag = tag;
                result = vkDebugMarkerSetObjectTagEXT(MAGMA_HANDLE(device), &objectTagInfo);
            }
        #endif // VK_EXT_debug_marker
    #ifdef VK_EXT_debug_utils
        }
    #endif
    }
    MAGMA_HANDLE_RESULT(result, "failed to set object tag");
}
#endif // MAGMA_DEBUG
} // namespace magma
