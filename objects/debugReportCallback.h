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
#include "../objects/handle.h"

namespace magma
{
    class Instance;
    class IAllocator;

    /* Debug report callbacks give more detailed feedback on the
       application's use of Vulkan when events of interest occur. */

    class DebugReportCallback : public NonDispatchable<VkDebugReportCallbackEXT>
    {
    public:
        explicit DebugReportCallback(std::shared_ptr<const Instance> instance,
            PFN_vkDebugReportCallbackEXT reportCallback,
            VkDebugReportFlagsEXT flags =
                VK_DEBUG_REPORT_INFORMATION_BIT_EXT |
                VK_DEBUG_REPORT_WARNING_BIT_EXT |
                VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
                VK_DEBUG_REPORT_ERROR_BIT_EXT |
                VK_DEBUG_REPORT_DEBUG_BIT_EXT,
            void *userData = nullptr,
            std::shared_ptr<IAllocator> allocator = nullptr);
        ~DebugReportCallback();

    private:
        std::shared_ptr<const Instance> instance;
    };
} // namespace magma
