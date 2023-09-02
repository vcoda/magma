/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2022 Victor Coda.

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
#ifdef _MSC_VER
#pragma once
#endif

#include <cstring>
#include <cstdarg>

#include <memory>
#include <vector>
#include <array>
#include <list>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <string>
#include <initializer_list>
#include <functional>
#include <algorithm>
#include <limits>
#include <future>

#include <atomic>
#include <mutex>

#ifdef _WIN32
  #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
  #endif
  #ifndef NOMINMAX
    #define NOMINMAX
  #endif
#endif // _WIN32

#ifndef VULKAN_H_
  #include <vulkan/vulkan.h>
#endif

#ifdef MAGMA_SSE
  #include <xmmintrin.h>
  #include <smmintrin.h>
#endif

#include "core.h"
#include "hash.h"
