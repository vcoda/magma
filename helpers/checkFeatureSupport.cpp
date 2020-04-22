/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2019 Victor Coda.

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
#include "stringize.h"
#include "../objects/surface.h"
#include "../objects/physicalDevice.h"
#include "../misc/exception.h"

namespace magma
{
namespace helpers
{
void checkImageUsageSupport(std::shared_ptr<const Surface> surface,
    VkImageUsageFlags usage,
    std::shared_ptr<const PhysicalDevice> physicalDevice)
{
    const VkImageUsageFlagBits usageFlags[] = {
        VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
        VK_IMAGE_USAGE_TRANSFER_DST_BIT,
        VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_IMAGE_USAGE_STORAGE_BIT,
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
        VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT,
        VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT,
#ifdef VK_NV_shading_rate_image
        VK_IMAGE_USAGE_SHADING_RATE_IMAGE_BIT_NV,
#endif
        VK_IMAGE_USAGE_FRAGMENT_DENSITY_MAP_BIT_EXT
    };
    const VkSurfaceCapabilitiesKHR surfaceCaps = physicalDevice->getSurfaceCapabilities(surface);
    for (const VkImageUsageFlagBits flag : usageFlags)
    {
        if (usage & flag)
        {
            if (!(surfaceCaps.supportedUsageFlags & flag))
                MAGMA_THROW(std::string(helpers::stringize(flag)) + "\nusage flag not supported by surface");
        }
    }
}
} // namespace helpers
} // namespace magma
