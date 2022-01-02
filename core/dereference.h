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
namespace core
{
template<typename Type>
inline typename Type::NativeHandle dereference(const std::shared_ptr<Type>& p)
{
    if (p) return *p;
#ifdef VK_NULL_HANDLE
    return VK_NULL_HANDLE;
#else
    return 0;
#endif
}

template<typename NativeHandle>
inline NativeHandle reinterpret(const void *p)
{
    if (p) return *reinterpret_cast<const NativeHandle *>(p);
#ifdef VK_NULL_HANDLE
    return VK_NULL_HANDLE;
#else
    return 0;
#endif
}
} // namespace core
} // namespace magma
