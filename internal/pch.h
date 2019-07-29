#pragma once
#include <cstddef>
#include <cstring>
#include <cstdarg>
#include <cassert>

#include <memory>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <initializer_list>
#include <stdexcept>
#include <limits>

#include <atomic>
#include <thread>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#endif
#ifndef VULKAN_H_
#include <vulkan/vulkan.h>
#endif

#include "shared.h"
#include "hash.h"
