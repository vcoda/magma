/*
Magma - C++1x interface over Khronos Vulkan API.
Copyright (C) 2018 Victor Coda.

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
namespace magma
{
template<typename Function>
MAGMA_INLINE InstanceExtension<Function>::InstanceExtension(VkInstance instance, const char *name) noexcept
{
    if (!addr)
    {
        MAGMA_ASSERT(instance != VK_NULL_HANDLE);
        MAGMA_ASSERT(name);
        addr = vkGetInstanceProcAddr(instance, name);
    }
}

template<typename Function>
MAGMA_INLINE InstanceExtension<Function>::InstanceExtension(VkInstance instance, const char *name, const char *extension)
{
    if (!addr)
    {
        MAGMA_ASSERT(instance != VK_NULL_HANDLE);
        MAGMA_ASSERT(name);
        addr = vkGetInstanceProcAddr(instance, name);
        if (!addr)
            MAGMA_THROW_NOT_PRESENT(extension);
    }
}

template<typename Function>
PFN_vkVoidFunction InstanceExtension<Function>::addr;
} // namespace magma
