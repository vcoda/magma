#pragma once
#ifdef _WIN32
// Turn off a bunch of stuff when windows.h is included in vk_platform.h
#define WIN32_LEAN_AND_MEAN
#define NOGDI             // All GDI defines and routines
#define NOKERNEL          // All KERNEL defines and routines
#define NOUSER            // All USER defines and routines
#define NONLS             // All NLS defines and routines
#define NOMINMAX
#define NOSERVICE
#define NOMCX
#define NOIME
#endif // _WIN32
#ifndef VULKAN_H_
#include <vulkan/vulkan.h>
#include "VK_NVX_raytracing.h"
#endif