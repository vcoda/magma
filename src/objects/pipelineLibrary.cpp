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
#include "pch.h"
#pragma hdrstop
#include "pipelineLibrary.h"
#include "device.h"
#include "../allocator/allocator.h"

namespace magma
{
#ifdef VK_KHR_pipeline_library
PipelineLibrary::PipelineLibrary(std::shared_ptr<Device> device, std::shared_ptr<IAllocator> allocator) noexcept:
    device(std::move(device)),
    allocator(std::move(allocator))
{}

PipelineLibrary::~PipelineLibrary()
{
    for (auto handle: libraries)
        vkDestroyPipeline(getNativeDevice(), handle, MAGMA_OPTIONAL(allocator));
}

VkDevice PipelineLibrary::getNativeDevice() const noexcept
{
    return device->getHandle();
}
#endif // VK_KHR_pipeline_library
} // namespace magma
