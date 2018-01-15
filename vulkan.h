#pragma once
#ifdef _WIN32
    // Turn off a bunch of stuff when windows.h is included in vk_platform.h
#   define WIN32_LEAN_AND_MEAN
#   define NOGDI
#   define NOMINMAX
#endif // _WIN32
#ifndef VULKAN_H_
#   include <vulkan/vulkan.h>
#endif