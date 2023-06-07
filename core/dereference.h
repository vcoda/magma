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
#pragma once

namespace magma
{
#if (VK_USE_64_BIT_PTR_DEFINES == 1)
typedef void *NonDispatchableHandle;
#else
typedef uint64_t NonDispatchableHandle;
#endif // VK_USE_64_BIT_PTR_DEFINES == 1

namespace core
{
template<class Type>
inline typename Type::NativeHandle dereference(const std::shared_ptr<Type>& ptr) noexcept
{
    if (ptr)
        return *ptr;
#ifdef VK_NULL_HANDLE
    return VK_NULL_HANDLE;
#else
    return 0;
#endif
}

template<class NativeHandle>
inline NativeHandle reinterpret(NonDispatchableHandle handle) noexcept
{
    if (handle)
    {
    #if (VK_USE_64_BIT_PTR_DEFINES == 1)
        return reinterpret_cast<NativeHandle>(handle);
    #else
        return static_cast<NativeHandle>(handle);
    #endif // VK_USE_64_BIT_PTR_DEFINES == 1
    }
#ifdef VK_NULL_HANDLE
    return VK_NULL_HANDLE;
#else
    return 0;
#endif
}
} // namespace core
} // namespace magma
