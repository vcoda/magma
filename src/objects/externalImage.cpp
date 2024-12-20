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
#include "externalImage.h"
#include "device.h"
#include "deviceMemory.h"
#include "../platform/androidHardwareBuffer.h"
#include "../allocator/allocator.h"
#include "../exceptions/errorResult.h"

namespace magma
{
#ifdef VK_ANDROID_external_memory_android_hardware_buffer
ExternalImage2D::ExternalImage2D(std::shared_ptr<Device> device, lent_ptr<AndroidHardwareBuffer> hardwareBuffer,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Sharing& sharing /* Sharing() */):
    Image2D(device, VK_NULL_HANDLE, hardwareBuffer->getFormat(), hardwareBuffer->getExtent())
{
    VkImageCreateInfo imageInfo;
    VkExternalMemoryImageCreateInfo externalMemoryImageInfo;
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.pNext = &externalMemoryImageInfo;
    imageInfo.flags = hardwareBuffer->protectedContent() ? VK_IMAGE_CREATE_PROTECTED_BIT : 0;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.format = format;
    imageInfo.extent = extent;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = hardwareBuffer->getLayers();
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.usage = hardwareBuffer->getImageUsage();
    imageInfo.sharingMode = sharing.getMode();
    imageInfo.queueFamilyIndexCount = sharing.getQueueFamiliesCount();
    imageInfo.pQueueFamilyIndices = sharing.getQueueFamilyIndices().data();
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    externalMemoryImageInfo.sType = VK_STRUCTURE_TYPE_EXTERNAL_MEMORY_IMAGE_CREATE_INFO;
    externalMemoryImageInfo.pNext = nullptr;
    externalMemoryImageInfo.handleTypes = VK_EXTERNAL_MEMORY_HANDLE_TYPE_ANDROID_HARDWARE_BUFFER_BIT_ANDROID;
    VkExternalFormatANDROID externalFormat;
    if (VK_FORMAT_UNDEFINED == imageInfo.format)
    {
        externalMemoryImageInfo.pNext = &externalFormat;
        externalFormat.sType = VK_STRUCTURE_TYPE_EXTERNAL_FORMAT_ANDROID;
        externalFormat.pNext = nullptr;
        externalFormat.externalFormat = hardwareBuffer->getFormatProperties().externalFormat;
    }
    const VkResult result = vkCreateImage(getNativeDevice(), &imageInfo, MAGMA_OPTIONAL_INSTANCE(hostAllocator), &handle);
    MAGMA_HANDLE_RESULT(result, "failed to create external image");
    std::unique_ptr<IDeviceMemory> memory = std::make_unique<DeviceMemory>(
        std::move(device), std::move(hardwareBuffer),
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        MAGMA_HOST_ALLOCATOR(allocator));
    bindMemory(std::move(memory));
}
#endif // VK_ANDROID_external_memory_android_hardware_buffer
} // namespace magma
