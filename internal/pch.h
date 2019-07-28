#pragma once
#include <cstddef>
#include <cstring>
#include <cstdarg>

#include <algorithm>
#include <functional>
#include <memory>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <string>
#include <initializer_list>
#include <stdexcept>
#include <limits>
#include <utility>

#include <atomic>
#include <thread>

#ifdef _MSC_VER
#include <malloc.h>
#else
#include <mm_malloc.h>
#endif

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#endif
#ifndef VULKAN_H_
#include <vulkan/vulkan.h>
#endif

#include "shared.h"
#include "noncopyable.h"
#include "hash.h"
