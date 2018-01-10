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
#include "swapchain.h"
#include "device.h"
#include "surface.h"
#include "image.h"
#include "queue.h"
#include "semaphore.h"
#include "fence.h"
#include "../shared.h"

namespace magma
{
Swapchain::Swapchain(std::shared_ptr<const Device> device, std::shared_ptr<const Surface> surface, uint32_t minImageCount, VkSurfaceFormatKHR surfaceFormat, const VkExtent2D& imageExtent,
	VkPresentModeKHR presentMode, /* VK_PRESENT_MODE_FIFO_KHR */
	VkSwapchainCreateFlagsKHR flags /* 0 */):
    NonDispatchable(VK_DEBUG_REPORT_OBJECT_TYPE_SWAPCHAIN_KHR_EXT, device),
    surfaceFormat(surfaceFormat)
{
	VkSwapchainCreateInfoKHR info;
	info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	info.pNext = nullptr;
	info.flags = flags;
	info.surface = *surface;
	info.minImageCount = minImageCount;
	info.imageFormat = surfaceFormat.format;
	info.imageColorSpace = surfaceFormat.colorSpace;
	info.imageExtent = imageExtent;
	info.imageArrayLayers = 1;
	info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	info.queueFamilyIndexCount = 0;
	info.pQueueFamilyIndices = nullptr;
	info.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	info.presentMode = presentMode;
	info.clipped = VK_TRUE;
	info.oldSwapchain = VK_NULL_HANDLE;
	const VkResult create = vkCreateSwapchainKHR(*device, &info, nullptr, &handle);
    MAGMA_THROW_FAILURE(create, "failed to create swapchain");
}

Swapchain::~Swapchain()
{
	vkDestroySwapchainKHR(*device, handle, nullptr);
}

uint32_t Swapchain::acquireNextImage(std::shared_ptr<const Semaphore> semaphore, std::shared_ptr<const Fence> fence) noexcept
{
    uint32_t imageIndex = 0;
    const VkResult acquire = vkAcquireNextImageKHR(*device, handle, UINT64_MAX, 
        MAGMA_OPTIONAL_HANDLE(semaphore),
        MAGMA_OPTIONAL_HANDLE(fence),
        &imageIndex);
    MAGMA_REPORT_FAILURE(acquire, "swapchain failed to acquire next image");
    return imageIndex;
}

uint32_t Swapchain::getImageCount() const noexcept
{
    uint32_t imageCount = 0;
    const VkResult get = vkGetSwapchainImagesKHR(*device, handle, &imageCount, nullptr);
    MAGMA_REPORT_FAILURE(get, "failed to get swapchain image count");
    return imageCount;
}

const std::vector<std::shared_ptr<Image>>& Swapchain::getImages() const noexcept
{
    if (images.empty())
    {
        uint32_t imageCount = getImageCount();
        if (imageCount > 0)
        {
            std::vector<VkImage> nativeImages(imageCount);
            const VkResult get = vkGetSwapchainImagesKHR(*device, handle, &imageCount, nativeImages.data());
            if (VK_SUCCESS == get)
            {
                for (const VkImage handle : nativeImages)
                {
                    std::shared_ptr<Image> image(new SwapchainImage(device, handle, this->surfaceFormat.format));
                    images.push_back(image);
                }
            }
        }
    }
    return images;
}
} // namespace magma
