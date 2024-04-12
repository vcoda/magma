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
#include <sstream>
#include "swapchain.h"
#include "swapchainImage.h"
#include "device.h"
#include "physicalDevice.h"
#include "surface.h"
#include "semaphore.h"
#include "fence.h"
#include "debugReportCallback.h"
#include "debugUtilsMessenger.h"
#include "../allocator/allocator.h"
#include "../misc/deviceFeatures.h"
#include "../misc/extension.h"
#include "../helpers/stackArray.h"
#include "../helpers/streamInsertOperators.h"
#include "../helpers/stringifyFlags.h"
#include "../exceptions/errorResult.h"

namespace magma
{
#ifdef VK_KHR_swapchain
Swapchain::Swapchain(std::shared_ptr<Device> device, VkSurfaceFormatKHR surfaceFormat,
    const VkExtent2D& extent, uint32_t arrayLayers, VkImageUsageFlags imageUsage,
    VkPresentModeKHR presentMode, VkSwapchainCreateFlagsKHR flags, const Sharing& sharing,
    std::shared_ptr<Swapchain> oldSwapchain, std::shared_ptr<IAllocator> allocator):
    NonDispatchable(VK_OBJECT_TYPE_SWAPCHAIN_KHR, std::move(device), std::move(allocator)),
    surfaceFormat(surfaceFormat),
    extent(extent),
    arrayLayers(arrayLayers),
    imageUsage(imageUsage),
    presentMode(presentMode),
    flags(flags),
    sharing(sharing),
    retired(false),
    imageIndex(0)
{
#ifndef MAGMA_NO_EXCEPTIONS
    if (oldSwapchain && oldSwapchain->hadRetired())
        throw exception::OutOfDate("old swapchain must be a non-retired");
#endif // !MAGMA_NO_EXCEPTIONS
}

Swapchain::Swapchain(std::shared_ptr<Device> device_, std::shared_ptr<const Surface> surface,
    uint32_t minImageCount, VkSurfaceFormatKHR surfaceFormat, const VkExtent2D& extent, uint32_t arrayLayers,
    VkImageUsageFlags imageUsage, VkSurfaceTransformFlagBitsKHR preTransform, VkCompositeAlphaFlagBitsKHR compositeAlpha,
    VkPresentModeKHR presentMode,
#ifdef VK_KHR_device_group
    VkDeviceGroupPresentModeFlagsKHR deviceGroupPresentModes /* 0 */,
#endif
    VkSwapchainCreateFlagsKHR flags /* 0 */,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    std::shared_ptr<Swapchain> oldSwapchain /* nullptr */,
#ifdef VK_EXT_debug_report
    std::shared_ptr<DebugReportCallback> debugReportCallback /* nullptr */,
#endif
#ifdef VK_EXT_debug_utils
    std::shared_ptr<DebugUtilsMessenger> debugUtilsMessenger /* nullptr */,
#endif
    const Sharing& sharing /* default */,
    const StructureChain& extendedInfo /* default */):
    Swapchain(std::move(device_), surfaceFormat, extent, arrayLayers, imageUsage, presentMode, flags, sharing, oldSwapchain, std::move(allocator))
{
    VkSwapchainCreateInfoKHR swapchainInfo;
    swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainInfo.pNext = extendedInfo.chainNodes();
    swapchainInfo.flags = flags;
    swapchainInfo.surface = *surface;
    swapchainInfo.minImageCount = minImageCount;
    swapchainInfo.imageFormat = surfaceFormat.format;
    swapchainInfo.imageColorSpace = surfaceFormat.colorSpace;
    swapchainInfo.imageExtent = extent;
    swapchainInfo.imageArrayLayers = arrayLayers;
    swapchainInfo.imageUsage = imageUsage;
    swapchainInfo.imageSharingMode = sharing.getMode();
    swapchainInfo.queueFamilyIndexCount = sharing.getQueueFamiliesCount();
    swapchainInfo.pQueueFamilyIndices = sharing.getQueueFamilyIndices().data();
    swapchainInfo.preTransform = preTransform;
    swapchainInfo.compositeAlpha = compositeAlpha;
    swapchainInfo.presentMode = presentMode;
    if (imageUsage & VK_IMAGE_USAGE_TRANSFER_SRC_BIT) // Is read back allowed?
        swapchainInfo.clipped = VK_FALSE; // Presentable images will own all of the pixels they contain
    else
        swapchainInfo.clipped = VK_TRUE; // Fragment shaders may not execute for obscured pixels
    swapchainInfo.oldSwapchain = MAGMA_OPTIONAL_HANDLE(oldSwapchain);
#ifdef VK_KHR_device_group
    VkDeviceGroupSwapchainCreateInfoKHR swapchainDeviceGroupInfo;
    if (device->extensionEnabled(VK_KHR_DEVICE_GROUP_EXTENSION_NAME))
    {
        swapchainDeviceGroupInfo.sType = VK_STRUCTURE_TYPE_DEVICE_GROUP_SWAPCHAIN_CREATE_INFO_KHR;
        swapchainDeviceGroupInfo.pNext = nullptr;
        swapchainDeviceGroupInfo.modes = deviceGroupPresentModes;
        linkNode(swapchainInfo, swapchainDeviceGroupInfo);
    }
#endif // VK_KHR_device_group
    if (!device->getFeatures()->supportsImageUsage(surface, swapchainInfo.imageUsage))
        MAGMA_ERROR("swapchain usage not supported by surface");
    VkResult result;
#if defined(VK_KHR_display_swapchain) && defined(VK_KHR_display_surface)
    if (std::dynamic_pointer_cast<const DisplaySurface>(surface))
    {
        MAGMA_REQUIRED_DEVICE_EXTENSION(vkCreateSharedSwapchainsKHR, VK_KHR_DISPLAY_SWAPCHAIN_EXTENSION_NAME);
        result = vkCreateSharedSwapchainsKHR(MAGMA_HANDLE(device), 1, &swapchainInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    }
    else
#endif // VK_KHR_display_swapchain && VK_KHR_display_surface
    {
        result = vkCreateSwapchainKHR(MAGMA_HANDLE(device), &swapchainInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    }
    if (oldSwapchain)
    {   // oldSwapchain is retired even if creation of the new swapchain fails
        oldSwapchain->retired = true;
    }
#if defined(VK_EXT_debug_report) || defined(VK_EXT_debug_utils)
    if (result != VK_SUCCESS)
    {
        std::ostringstream out;
        out << swapchainInfo << std::endl;
    #ifdef VK_KHR_device_group
        if (device->extensionEnabled(VK_KHR_DEVICE_GROUP_EXTENSION_NAME))
            out << swapchainDeviceGroupInfo << std::endl;
    #endif
    #ifdef VK_EXT_debug_report
        if (debugReportCallback)
        {
            debugReportCallback->message(VK_DEBUG_REPORT_ERROR_BIT_EXT,
                getObjectType(), (uint64_t)VK_NULL_HANDLE, 0, 0,
                "magma", out.str().c_str());
        }
    #endif // VK_EXT_debug_report
    #ifdef VK_EXT_debug_utils
        if (debugUtilsMessenger)
        {
            debugUtilsMessenger->message(VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
                VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT,
                "magma", 0, out.str().c_str());
        }
    #endif // VK_EXT_debug_utils
    }
#endif // VK_EXT_debug_report || VK_EXT_debug_utils
    handleError(result, "failed to create swapchain");
}

Swapchain::~Swapchain()
{
    vkDestroySwapchainKHR(MAGMA_HANDLE(device), handle, MAGMA_OPTIONAL_INSTANCE(hostAllocator));
}

uint32_t Swapchain::getImageCount() const
{
    uint32_t swapchainImageCount = 0;
    const VkResult result = vkGetSwapchainImagesKHR(MAGMA_HANDLE(device), handle, &swapchainImageCount, nullptr);
    MAGMA_HANDLE_RESULT(result, "failed to get swapchain image count");
    return swapchainImageCount;
}

const std::vector<std::shared_ptr<SwapchainImage>>& Swapchain::getImages() const
{
    if (bindedImages.empty())
    {
        uint32_t swapchainImageCount = 0;
        vkGetSwapchainImagesKHR(MAGMA_HANDLE(device), handle, &swapchainImageCount, nullptr);
        if (swapchainImageCount)
        {
            MAGMA_STACK_ARRAY(VkImage, swapchainImages, swapchainImageCount);
            const VkResult result = vkGetSwapchainImagesKHR(MAGMA_HANDLE(device), handle, &swapchainImageCount, swapchainImages);
            MAGMA_HANDLE_RESULT(result, "failed to get swapchain images");
            uint32_t imageIndex = 0;
            for (VkImage handle: swapchainImages)
            {   // Image has been created by swapchain internally, so we just assign image handle
                bindedImages.emplace_back(SwapchainImage::makeShared(device, handle, surfaceFormat.format, extent, imageIndex));
                ++imageIndex;
            }
        }
    }
    return bindedImages;
}

uint32_t Swapchain::acquireNextImage(std::shared_ptr<const Semaphore> semaphore /* nullptr */,
    std::shared_ptr<const Fence> fence /* nullptr */,
    uint64_t timeout /* std::numeric_limits<uint64_t>::max() */)
{
    uint32_t imageIndex = 0;
    const VkResult result = vkAcquireNextImageKHR(MAGMA_HANDLE(device), handle, timeout,
        MAGMA_OPTIONAL_HANDLE(semaphore), MAGMA_OPTIONAL_HANDLE(fence), &imageIndex);
    handleError(result, "failed to acquire next image");
    return imageIndex;
}

#ifdef VK_KHR_device_group
uint32_t Swapchain::acquireNextDeviceGroupImage(uint32_t deviceMask,
    std::shared_ptr<const Semaphore> semaphore /* nullptr */,
    std::shared_ptr<const Fence> fence /* nullptr */,
    uint64_t timeout /* std::numeric_limits<uint64_t>::max() */)
{
    VkAcquireNextImageInfoKHR aquireNextImageInfo;
    aquireNextImageInfo.sType = VK_STRUCTURE_TYPE_ACQUIRE_NEXT_IMAGE_INFO_KHR;
    aquireNextImageInfo.pNext = nullptr;
    aquireNextImageInfo.swapchain = handle;
    aquireNextImageInfo.timeout = timeout;
    aquireNextImageInfo.semaphore = MAGMA_OPTIONAL_HANDLE(semaphore);
    aquireNextImageInfo.fence = MAGMA_OPTIONAL_HANDLE(fence);
    aquireNextImageInfo.deviceMask = deviceMask;
    uint32_t imageIndex = 0;
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkAcquireNextImage2KHR, VK_KHR_DEVICE_GROUP_EXTENSION_NAME);
    const VkResult result = vkAcquireNextImage2KHR(MAGMA_HANDLE(device), &aquireNextImageInfo, &imageIndex);
    handleError(result, "failed to acquire next image within device group");
    return imageIndex;
}
#endif // VK_KHR_device_group

#ifdef VK_KHR_bind_memory2
void Swapchain::bindImage(std::shared_ptr<SwapchainImage> image, uint32_t imageIndex)
{
    VkBindImageMemoryInfoKHR bindImageMemoryInfo;
    VkBindImageMemorySwapchainInfoKHR bindImageMemorySwapchainInfo;
    bindImageMemoryInfo.sType = VK_STRUCTURE_TYPE_BIND_IMAGE_MEMORY_INFO_KHR;
    bindImageMemoryInfo.pNext = &bindImageMemorySwapchainInfo;
    bindImageMemoryInfo.image = *image;
    bindImageMemoryInfo.memory = VK_NULL_HANDLE;
    bindImageMemoryInfo.memoryOffset = 0;
    bindImageMemorySwapchainInfo.sType = VK_STRUCTURE_TYPE_BIND_IMAGE_MEMORY_SWAPCHAIN_INFO_KHR;
    bindImageMemorySwapchainInfo.pNext = nullptr;
    bindImageMemorySwapchainInfo.swapchain = handle;
    bindImageMemorySwapchainInfo.imageIndex = imageIndex;
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkBindImageMemory2KHR, VK_KHR_BIND_MEMORY_2_EXTENSION_NAME);
    const VkResult result = vkBindImageMemory2KHR(MAGMA_HANDLE(device), 1, &bindImageMemoryInfo);
    MAGMA_HANDLE_RESULT(result, "failed to bind image to swapchain");
    addImage(std::move(image), imageIndex);
}

#ifdef VK_KHR_device_group
void Swapchain::bindImage(std::shared_ptr<SwapchainImage> image, uint32_t imageIndex,
    const std::vector<uint32_t>& deviceIndices,
    const std::vector<VkRect2D>& splitInstanceBindRegions /* empty */)
{
    VkBindImageMemoryInfoKHR bindImageMemoryInfo;
    VkBindImageMemoryDeviceGroupInfoKHR bindImageMemoryDeviceGroupInfo;
    VkBindImageMemorySwapchainInfoKHR bindImageMemorySwapchainInfo;
    bindImageMemoryInfo.sType = VK_STRUCTURE_TYPE_BIND_IMAGE_MEMORY_INFO_KHR;
    bindImageMemoryInfo.pNext = &bindImageMemoryDeviceGroupInfo;
    bindImageMemoryInfo.image = *image;
    bindImageMemoryInfo.memory = VK_NULL_HANDLE;
    bindImageMemoryInfo.memoryOffset = 0;
    bindImageMemoryDeviceGroupInfo.sType = VK_STRUCTURE_TYPE_BIND_IMAGE_MEMORY_DEVICE_GROUP_INFO_KHR;
    bindImageMemoryDeviceGroupInfo.pNext = &bindImageMemorySwapchainInfo;
    bindImageMemoryDeviceGroupInfo.deviceIndexCount = MAGMA_COUNT(deviceIndices);
    bindImageMemoryDeviceGroupInfo.pDeviceIndices = deviceIndices.data();
    bindImageMemoryDeviceGroupInfo.splitInstanceBindRegionCount = MAGMA_COUNT(splitInstanceBindRegions);
    bindImageMemoryDeviceGroupInfo.pSplitInstanceBindRegions = splitInstanceBindRegions.data();
    bindImageMemorySwapchainInfo.sType = VK_STRUCTURE_TYPE_BIND_IMAGE_MEMORY_SWAPCHAIN_INFO_KHR;
    bindImageMemorySwapchainInfo.pNext = nullptr;
    bindImageMemorySwapchainInfo.swapchain = handle;
    bindImageMemorySwapchainInfo.imageIndex = imageIndex;
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkBindImageMemory2KHR, VK_KHR_BIND_MEMORY_2_EXTENSION_NAME);
    const VkResult result = vkBindImageMemory2KHR(MAGMA_HANDLE(device), 1, &bindImageMemoryInfo);
    MAGMA_HANDLE_RESULT(result, "failed to bind image to swapchain within device group");
    addImage(std::move(image), imageIndex);
}
#endif // VK_KHR_device_group
#endif // VK_KHR_bind_memory2

#ifdef VK_AMD_display_native_hdr
void Swapchain::setLocalDimming(bool enable) noexcept
{
    MAGMA_DEVICE_EXTENSION(vkSetLocalDimmingAMD);
    if (vkSetLocalDimmingAMD)
        vkSetLocalDimmingAMD(MAGMA_HANDLE(device), handle, MAGMA_BOOLEAN(enable));
}
#endif // VK_AMD_display_native_hdr

void Swapchain::addImage(std::shared_ptr<SwapchainImage> image, uint32_t imageIndex)
{
    image->setChainIndex(imageIndex);
    const uint32_t imageCount = imageIndex + 1;
    if (imageCount > bindedImages.size())
        bindedImages.resize(imageCount);
    bindedImages[imageIndex] = std::move(image);
}

void Swapchain::handleError(VkResult result, const char *message) const
{
#ifndef MAGMA_NO_EXCEPTIONS
    switch (result)
    {
    case VK_ERROR_INITIALIZATION_FAILED:
        throw exception::InitializationFailed(message);
    case VK_ERROR_DEVICE_LOST:
        throw exception::DeviceLost(message, MAGMA_SOURCE_LOCATION);
#ifdef VK_KHR_surface
    case VK_ERROR_SURFACE_LOST_KHR:
        throw exception::SurfaceLost(message);
    case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
        throw exception::NativeWindowInUse(message);
#endif
#ifdef VK_KHR_swapchain
    case VK_ERROR_OUT_OF_DATE_KHR:
        throw exception::OutOfDate(message);
#endif
#ifdef VK_KHR_display_swapchain
    case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
        throw exception::IncompatibleDisplay(message);
#endif
#ifdef VK_EXT_full_screen_exclusive
    case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT:
        throw exception::FullScreenExclusiveModeLost(message);
#endif
    }
#endif // !MAGMA_NO_EXCEPTIONS
    MAGMA_HANDLE_RESULT(result, message);
}
#endif // VK_KHR_swapchain
} // namespace magma

#ifdef VK_KHR_swapchain
std::ostream& operator<<(std::ostream& out, const VkSwapchainCreateInfoKHR& info)
{
    out << "VkSwapchainCreateInfoKHR [" << std::endl
        << "\tflags: " << magma::helpers::stringifySwapchainFlags(info.flags) << std::endl
        << "\tsurface: 0x" << std::hex << info.surface << std::dec << std::endl
        << "\tminImageCount: " << info.minImageCount << std::endl
        << "\timageFormat: " << info.imageFormat << std::endl
        << "\timageColorSpace: " << info.imageColorSpace << std::endl
        << "\timageExtent: [" << info.imageExtent.width << ", "
            << info.imageExtent.height << "]" << std::endl
        << "\timageArrayLayers: " << info.imageArrayLayers << std::endl
        << "\timageUsage: " << magma::helpers::stringifyImageUsage(info.imageUsage) << std::endl
        << "\timageSharingMode: " << info.imageSharingMode << std::endl
        << "\tqueueFamilyIndexCount: " << info.queueFamilyIndexCount << std::endl
        << "\tpQueueFamilyIndices: [";
    for (uint32_t i = 0; i < info.queueFamilyIndexCount; ++i)
    {
        MAGMA_ASSERT(info.pQueueFamilyIndices);
        out << info.pQueueFamilyIndices[i];
        if (i < info.queueFamilyIndexCount - 1)
            out << ", ";
    }
    out << "]" << std::endl
        << "\tpreTransform: " << info.preTransform << std::endl
        << "\tcompositeAlpha: " << info.compositeAlpha << std::endl
        << "\tpresentMode: " << info.presentMode << std::endl
        << "\tclipped: " << magma::Bool(info.clipped) << std::endl
        << "\toldSwapchain: 0x" << std::hex << info.oldSwapchain << std::dec << std::endl
        << "]";
    return out;
}

#ifdef VK_KHR_device_group
std::ostream& operator<<(std::ostream& out, const VkDeviceGroupSwapchainCreateInfoKHR& info)
{
    out << "VkDeviceGroupSwapchainCreateInfoKHR [" << std::endl
        << "\tmodes: " << magma::helpers::stringifyDeviceGroupPresentMode(info.modes) << std::endl
        << "]";
    return out;
}
#endif // VK_KHR_device_group
#endif // VK_KHR_swapchain
