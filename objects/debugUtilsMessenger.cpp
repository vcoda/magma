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
#include "pch.h"
#pragma hdrstop
#include <cstdarg>
#include "debugUtilsMessenger.h"
#include "instance.h"
#include "../allocator/allocator.h"
#include "../misc/instanceExtension.h"

namespace magma
{
DebugUtilsMessenger::DebugUtilsMessenger(std::shared_ptr<const Instance> instance,
    PFN_vkDebugUtilsMessengerCallbackEXT userCallback,
    VkDebugUtilsMessageSeverityFlagsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    void *userData /* nullptr */,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    NonDispatchable(VK_OBJECT_TYPE_DEBUG_UTILS_MESSENGER_EXT, nullptr, std::move(allocator)),
    instance(std::move(instance))
{
    MAGMA_ASSERT(userCallback);
    MAGMA_OPTIONAL_INSTANCE_EXTENSION(vkCreateDebugUtilsMessengerEXT);
    if (vkCreateDebugUtilsMessengerEXT)
    {
        VkDebugUtilsMessengerCreateInfoEXT info;
        info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        info.pNext = nullptr;
        info.flags = 0;
        info.messageSeverity = messageSeverity;
        info.messageType = messageType;
        info.pfnUserCallback = userCallback;
        info.pUserData = userData;
        const VkResult create = vkCreateDebugUtilsMessengerEXT(MAGMA_HANDLE(instance), &info, MAGMA_OPTIONAL_INSTANCE(allocator), &handle);
        MAGMA_THROW_FAILURE(create, "failed to create debug messenger");
    }
}

DebugUtilsMessenger::~DebugUtilsMessenger()
{
    MAGMA_OPTIONAL_INSTANCE_EXTENSION(vkDestroyDebugUtilsMessengerEXT);
    if (vkDestroyDebugUtilsMessengerEXT)
        vkDestroyDebugUtilsMessengerEXT(MAGMA_HANDLE(instance), handle, MAGMA_OPTIONAL_INSTANCE(allocator));
}

void DebugUtilsMessenger::message(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes,
    const char *messageIdName, int32_t messageIdNumber, const char *format, ...) const noexcept
{
    MAGMA_ASSERT(format);
    MAGMA_OPTIONAL_INSTANCE_EXTENSION(vkSubmitDebugUtilsMessageEXT);
    if (vkSubmitDebugUtilsMessageEXT)
    {
        char message[MAGMA_MAX_MESSAGE_STRING];
        va_list args;
        va_start(args, format);
#ifdef _MSC_VER
        vsprintf_s(message, format, args);
#else
        vsprintf(message, format, args);
#endif
        va_end(args);
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
        vkSubmitDebugUtilsMessageEXT(MAGMA_HANDLE(instance), messageSeverity, messageTypes, &data);
    }
}
} // namespace magma
