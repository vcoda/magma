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
#include <cstring>
#include <cmath>
#if defined(_MSC_VER) || defined(__MINGW32__)
    #include <malloc.h>
#else
    #include <mm_malloc.h>
#endif

/*
 * Standard Template Library
 */

#include <vector>
#include <deque>
#include <list>
#include <forward_list>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <array>
#include <initializer_list>
#include <string>
#include <memory>
#include <future>
#include <atomic>
#include <mutex>
#include <algorithm>
#include <functional>
#include <limits>
#include <iostream>

/*
 * Platform
 */

#if defined(_WIN32) || defined(WIN32) || defined(__WINDOWS__)
    #define WIN32_LEAN_AND_MEAN
    #ifndef NOMINMAX
        #define NOMINMAX 1
    #endif
#elif defined(__unix__) || defined(__unix)
    #include <unistd.h>
#endif // __unix__ || __unix

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
#include "../objects/dispatchable.h"
#include "../objects/nondispatchable.h"
#include "../objects/device.h"
#include "../allocator/allocator.h"
#include "../misc/extension.h"
#include "../misc/structureChain.h"
#include "../exceptions/errorResult.h"
#include "../helpers/stackArray.h"
