/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2024 Victor Coda.

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

/*
 * C Runtime
 */

#include <cassert>
#include <cmath>
#include <cstring>
#if defined(_MSC_VER) || defined(__MINGW32__)
    #include <malloc.h>
#else
    #include <mm_malloc.h>
#endif

/*
 * Standard Template Library
 */

#include <algorithm>
#include <array>
#include <atomic>
#include <deque>
#include <forward_list>
#include <functional>
#include <future>
#include <initializer_list>
#include <iostream>
#include <limits>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

/*
 * Platform
 */

#if defined(_WIN32) || defined(WIN32) || defined(__WINDOWS__)
    #define WIN32_LEAN_AND_MEAN
    #ifndef NOMINMAX
        #define NOMINMAX 1
    #endif
#elif defined(__unix__) || defined(__unix) || defined(__APPLE__)
    #include <unistd.h>
#endif // __unix__ || __unix || __APPLE__

/*
 * Vulkan SDK
 */

#include <vulkan/vulkan.h>

/*
 * SIMD
 */

#ifdef MAGMA_SSE
    #include <xmmintrin.h>
    #include <smmintrin.h>
#elif defined(MAGMA_NEON)
    #include <arm_neon.h>
#endif

/*
 * Magma Library
 */

#include "core.h"
#include "../objects/model/dispatchable.h"
#include "../objects/model/nondispatchable.h"
#include "../objects/device.h"
#include "../allocator/allocator.h"
#include "../misc/extension.h"
#include "../misc/structureChain.h"
#include "../exceptions/errorResult.h"
