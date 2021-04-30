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

    /* Debug report callbacks give more detailed feedback on the
       application's use of Vulkan when events of interest occur. */

#ifdef VK_EXT_debug_report
    class DebugReportCallback : public NonDispatchable<VkDebugReportCallbackEXT>
    {
    public:
        explicit DebugReportCallback(std::shared_ptr<const Instance> instance,
            PFN_vkDebugReportCallbackEXT userCallback,
            std::shared_ptr<IAllocator> allocator = nullptr,
            VkDebugReportFlagsEXT flags =
                VK_DEBUG_REPORT_INFORMATION_BIT_EXT |
                VK_DEBUG_REPORT_WARNING_BIT_EXT |
                VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
                VK_DEBUG_REPORT_ERROR_BIT_EXT |
                VK_DEBUG_REPORT_DEBUG_BIT_EXT,
            void *userData = nullptr);
        ~DebugReportCallback();
        void message(VkDebugReportFlagsEXT flags,
            VkObjectType objectType,
            uint64_t object,
            std::size_t location,
            int32_t messageCode,
            const char *layerPrefix,
            const char *format, ...) const noexcept;

    private:
        std::shared_ptr<const Instance> instance;
    };
#endif // VK_EXT_debug_report
} // namespace magma
