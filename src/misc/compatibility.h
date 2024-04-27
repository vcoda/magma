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
#pragma once

#if !defined(VK_VERSION_1_1)
typedef enum VkObjectType {
    VK_OBJECT_TYPE_UNKNOWN = 0,
    VK_OBJECT_TYPE_INSTANCE = 1,
    VK_OBJECT_TYPE_PHYSICAL_DEVICE = 2,
    VK_OBJECT_TYPE_DEVICE = 3,
    VK_OBJECT_TYPE_QUEUE = 4,
    VK_OBJECT_TYPE_SEMAPHORE = 5,
    VK_OBJECT_TYPE_COMMAND_BUFFER = 6,
    VK_OBJECT_TYPE_FENCE = 7,
    VK_OBJECT_TYPE_DEVICE_MEMORY = 8,
    VK_OBJECT_TYPE_BUFFER = 9,
    VK_OBJECT_TYPE_IMAGE = 10,
    VK_OBJECT_TYPE_EVENT = 11,
    VK_OBJECT_TYPE_QUERY_POOL = 12,
    VK_OBJECT_TYPE_BUFFER_VIEW = 13,
    VK_OBJECT_TYPE_IMAGE_VIEW = 14,
    VK_OBJECT_TYPE_SHADER_MODULE = 15,
    VK_OBJECT_TYPE_PIPELINE_CACHE = 16,
    VK_OBJECT_TYPE_PIPELINE_LAYOUT = 17,
    VK_OBJECT_TYPE_RENDER_PASS = 18,
    VK_OBJECT_TYPE_PIPELINE = 19,
    VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT = 20,
    VK_OBJECT_TYPE_SAMPLER = 21,
    VK_OBJECT_TYPE_DESCRIPTOR_POOL = 22,
    VK_OBJECT_TYPE_DESCRIPTOR_SET = 23,
    VK_OBJECT_TYPE_FRAMEBUFFER = 24,
    VK_OBJECT_TYPE_COMMAND_POOL = 25,
    VK_OBJECT_TYPE_SAMPLER_YCBCR_CONVERSION_KHR = 1000156000,
    VK_OBJECT_TYPE_DESCRIPTOR_UPDATE_TEMPLATE_KHR = 1000085000,
    VK_OBJECT_TYPE_PRIVATE_DATA_SLOT_EXT = 1000295000,
    VK_OBJECT_TYPE_SURFACE_KHR = 1000000000,
    VK_OBJECT_TYPE_SWAPCHAIN_KHR = 1000001000,
    VK_OBJECT_TYPE_DISPLAY_KHR = 1000002000,
    VK_OBJECT_TYPE_DISPLAY_MODE_KHR = 1000002001,
    VK_OBJECT_TYPE_DEBUG_REPORT_CALLBACK_EXT = 1000011000,
    VK_OBJECT_TYPE_CU_MODULE_NVX = 1000029000,
    VK_OBJECT_TYPE_CU_FUNCTION_NVX = 1000029001,
    VK_OBJECT_TYPE_OBJECT_TABLE_NVX = 1000086000,
    VK_OBJECT_TYPE_INDIRECT_COMMANDS_LAYOUT_NVX = 1000086001,
    VK_OBJECT_TYPE_DEBUG_UTILS_MESSENGER_EXT = 1000128000,
    VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_KHR = 1000150000,
    VK_OBJECT_TYPE_VALIDATION_CACHE_EXT = 1000160000,
    VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_NV = 1000165000,
    VK_OBJECT_TYPE_PERFORMANCE_CONFIGURATION_INTEL = 1000210000,
    VK_OBJECT_TYPE_DEFERRED_OPERATION_KHR = 1000268000,
    VK_OBJECT_TYPE_INDIRECT_COMMANDS_LAYOUT_NV = 1000277000,
    VK_OBJECT_TYPE_BUFFER_COLLECTION_FUCHSIA = 1000366000,
    VK_OBJECT_TYPE_MAX_ENUM = 0x7FFFFFFF
} VkObjectType;
#endif // !VK_VERSION_1_1

#if VK_HEADER_VERSION < 34 // TODO: exact version
#if defined(VK_EXT_debug_report) && defined(VK_KHR_display)
constexpr VkDebugReportObjectTypeEXT VK_DEBUG_REPORT_OBJECT_TYPE_DISPLAY_KHR_EXT = VkDebugReportObjectTypeEXT(29);
constexpr VkDebugReportObjectTypeEXT VK_DEBUG_REPORT_OBJECT_TYPE_DISPLAY_MODE_KHR_EXT = VkDebugReportObjectTypeEXT(30);
#endif // VK_EXT_debug_report && VK_KHR_display
#endif

#if !defined(VK_KHR_synchronization2)
constexpr VkAccessFlagBits VK_ACCESS_NONE_KHR = VkAccessFlagBits(0);
#endif

#if VK_HEADER_VERSION < 75
typedef struct VkBaseOutStructure {
    VkStructureType               sType;
    struct VkBaseOutStructure*    pNext;
} VkBaseOutStructure;

typedef struct VkBaseInStructure {
    VkStructureType                    sType;
    const struct VkBaseInStructure*    pNext;
} VkBaseInStructure;
#endif // VK_HEADER_VERSION < 75

#if VK_HEADER_VERSION < 184
typedef struct VkPipelineCacheHeaderVersionOne {
    uint32_t                        headerSize;
    VkPipelineCacheHeaderVersion    headerVersion;
    uint32_t                        vendorID;
    uint32_t                        deviceID;
    uint8_t                         pipelineCacheUUID[VK_UUID_SIZE];
} VkPipelineCacheHeaderVersionOne;
#endif // VK_HEADER_VERSION < 184

#if VK_HEADER_VERSION < 131
constexpr VkResult VK_ERROR_UNKNOWN = VkResult(-13);
#endif
