/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2023 Victor Coda.

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
#include <cstdarg>
#include "debugUtilsMessenger.h"
#include "instance.h"
#include "../allocator/allocator.h"
#include "../misc/extension.h"
#include "../exceptions/errorResult.h"

namespace magma
{
#ifdef VK_EXT_debug_utils
DebugUtilsMessenger::DebugUtilsMessenger(std::shared_ptr<Instance> instance_,
    PFN_vkDebugUtilsMessengerCallbackEXT userCallback,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    VkDebugUtilsMessageSeverityFlagsEXT messageSeverity /* VERBOSE_BIT | INFO_BIT | WARNING_BIT ERROR_BIT */,
    VkDebugUtilsMessageTypeFlagsEXT messageType /* GENERAL_BIT | VALIDATION_BIT | PERFORMANCE_BIT */,
    void *userData /* nullptr */,
    const StructureChain& extendedInfo /* default */):
    NonDispatchable(VK_OBJECT_TYPE_DEBUG_UTILS_MESSENGER_EXT, std::move(allocator)),
    instance(std::move(instance_))
{
    MAGMA_ASSERT(userCallback);
    MAGMA_INSTANCE_EXTENSION(vkCreateDebugUtilsMessengerEXT);
    if (vkCreateDebugUtilsMessengerEXT)
    {
        VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerInfo;
        debugUtilsMessengerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        debugUtilsMessengerInfo.pNext = extendedInfo.headNode();
        debugUtilsMessengerInfo.flags = 0;
        debugUtilsMessengerInfo.messageSeverity = messageSeverity;
        debugUtilsMessengerInfo.messageType = messageType;
        debugUtilsMessengerInfo.pfnUserCallback = userCallback;
        debugUtilsMessengerInfo.pUserData = userData;
        const VkResult result = vkCreateDebugUtilsMessengerEXT(*instance, &debugUtilsMessengerInfo,
            MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
        MAGMA_HANDLE_RESULT(result, "failed to create debug messenger");
    }
}

DebugUtilsMessenger::~DebugUtilsMessenger()
{
    MAGMA_INSTANCE_EXTENSION(vkDestroyDebugUtilsMessengerEXT);
    if (vkDestroyDebugUtilsMessengerEXT)
        vkDestroyDebugUtilsMessengerEXT(*instance, handle, MAGMA_OPTIONAL_INSTANCE(hostAllocator));
}

void DebugUtilsMessenger::message(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes,
    const char *messageIdName, int32_t messageIdNumber, const char *message) const noexcept
{
    MAGMA_INSTANCE_EXTENSION(vkSubmitDebugUtilsMessageEXT);
    if (vkSubmitDebugUtilsMessageEXT)
    {
        VkDebugUtilsMessengerCallbackDataEXT data;
        data.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CALLBACK_DATA_EXT;
        data.pNext = nullptr;
        data.flags = 0;
        data.pMessageIdName = messageIdName;
        data.messageIdNumber = messageIdNumber;
        data.pMessage = message;
        data.queueLabelCount = 0;
        data.pQueueLabels = nullptr;
        data.cmdBufLabelCount = 0;
        data.pCmdBufLabels = nullptr;
        data.objectCount = 0;
        data.pObjects = nullptr;
        vkSubmitDebugUtilsMessageEXT(*instance, messageSeverity, messageTypes, &data);
    }
}

void DebugUtilsMessenger::messageFormat(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes,
    const char *messageIdName, int32_t messageIdNumber, const char *format, ...) const noexcept
{
    char buffer[MAGMA_MAX_STRING];
    va_list args;
    va_start(args, format);
#if defined(_MSC_VER) || defined(__MINGW32__)
    vsnprintf_s(buffer, MAGMA_MAX_STRING, _TRUNCATE, format, args);
#else
    MAGMA_ASSERT(strlen(format) < MAGMA_MAX_STRING);
    vsprintf(buffer, format, args);
#endif // _MSC_VER || __MINGW32__
    va_end(args);
    message(messageSeverity, messageTypes, messageIdName, messageIdNumber, buffer);
}
#endif // VK_EXT_debug_utils
} // namespace magma
