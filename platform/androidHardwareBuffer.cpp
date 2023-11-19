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
#ifdef VK_USE_PLATFORM_ANDROID_KHR
#include <android/hardware_buffer.h>
#endif
#include "androidHardwareBuffer.h"
#include "../objects/device.h"
#include "../exceptions/errorResult.h"

#undef MAGMA_HANDLE
#define MAGMA_HANDLE(device) *device

#include "../misc/extension.h"

namespace magma
{
#ifdef VK_ANDROID_external_memory_android_hardware_buffer
AndroidHardwareBuffer::AndroidHardwareBuffer(std::shared_ptr<Device> device, AHardwareBuffer* buffer):
    buffer(buffer)
{
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    AHardwareBuffer_describe(buffer, &bufferDesc);
#endif
    properties.sType = VK_STRUCTURE_TYPE_ANDROID_HARDWARE_BUFFER_PROPERTIES_ANDROID;
    properties.pNext = &formatProperties;
    formatProperties.sType = VK_STRUCTURE_TYPE_ANDROID_HARDWARE_BUFFER_FORMAT_PROPERTIES_ANDROID;
    MAGMA_REQUIRED_DEVICE_EXTENSION(vkGetAndroidHardwareBufferPropertiesANDROID, VK_ANDROID_EXTERNAL_MEMORY_ANDROID_HARDWARE_BUFFER_EXTENSION_NAME);
    const VkResult result = vkGetAndroidHardwareBufferPropertiesANDROID(MAGMA_HANDLE(device), buffer, &properties);
    MAGMA_HANDLE_RESULT(result, "failed to get properties of android hardware buffer");
}

VkExtent2D AndroidHardwareBuffer::getExtent() const noexcept
{
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    VkExtent2D extent;
    extent.width = bufferDesc.width;
    extent.height = bufferDesc.height;
    return extent;
#endif // VK_USE_PLATFORM_ANDROID_KHR
    return VkExtent2D{0, 0};
}

uint32_t AndroidHardwareBuffer::getLayers() const noexcept
{
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    return bufferDesc.layers;
#endif // VK_USE_PLATFORM_ANDROID_KHR
    return 1;
}

VkImageUsageFlags AndroidHardwareBuffer::getImageUsage() const noexcept
{
    VkImageUsageFlags usage = 0;
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    if (bufferDesc.usage & AHARDWAREBUFFER_USAGE_GPU_SAMPLED_IMAGE)
        usage |= VK_IMAGE_USAGE_SAMPLED_BIT;
    if (bufferDesc.usage & AHARDWAREBUFFER_USAGE_GPU_COLOR_OUTPUT)
        usage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
#endif // VK_USE_PLATFORM_ANDROID_KHR
    return usage;
}

bool AndroidHardwareBuffer::protectedContent() const noexcept
{
#ifdef VK_USE_PLATFORM_ANDROID_KHR
    return (bufferDesc.usage & AHARDWAREBUFFER_USAGE_PROTECTED_CONTENT);
#else
    return false;
#endif // VK_USE_PLATFORM_ANDROID_KHR
}

VkMemoryRequirements AndroidHardwareBuffer::getMemoryRequirements() const noexcept
{
    VkMemoryRequirements memoryRequirements;
    memoryRequirements.size = properties.allocationSize;
    memoryRequirements.alignment = 0; // TODO: What is a proper alignment?
    memoryRequirements.memoryTypeBits = properties.memoryTypeBits;
    return memoryRequirements;
}
#endif // VK_ANDROID_external_memory_android_hardware_buffer
} // namespace magma
