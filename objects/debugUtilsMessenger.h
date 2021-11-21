/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2020 Victor Coda.

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
#include "nondispatchable.h"

namespace magma
{
    class Instance;
    class IAllocator;

    /* Due to the nature of the Vulkan interface, there is very little error information
       available to the developer and application. By using the VK_EXT_debug_utils extension,
       developers can obtain more information. When combined with validation layers,
       even more detailed feedback on the application's use of Vulkan will be provided. */

#ifdef VK_EXT_debug_utils
    class DebugUtilsMessenger : public NonDispatchable<VkDebugUtilsMessengerEXT>
    {
    public:
        explicit DebugUtilsMessenger(std::shared_ptr<const Instance> instance,
            PFN_vkDebugUtilsMessengerCallbackEXT userCallback,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkDebugUtilsMessageSeverityFlagsEXT messageSeverity =
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
            VkDebugUtilsMessageTypeFlagsEXT messageType =
                VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
            void *userData = nullptr);
        ~DebugUtilsMessenger();
        void message(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageTypes,
            const char *messageIdName,
            int32_t messageIdNumber,
            const char *format, ...) const noexcept;

    private:
        std::shared_ptr<const Instance> instance;
    };
#endif // VK_EXT_debug_utils
} // namespace magma
