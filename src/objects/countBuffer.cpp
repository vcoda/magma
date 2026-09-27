/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2026 Victor Coda.

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
#include "countBuffer.h"
#include "commandBuffer.h"

namespace magma
{
DispatchCountBuffer::DispatchCountBuffer(std::shared_ptr<Device> device, VkPipelineStageFlags stageMask,
    std::shared_ptr<Allocator> allocator /* nullptr */,
    const Sharing& sharing /* default */) :
    ReadbackBuffer<VkDispatchIndirectCommand>(std::move(device), stageMask, std::move(allocator), 1, true, Initializer(), sharing)
{}

void DispatchCountBuffer::setDispatch(uint32_t x, uint32_t y, uint32_t z, lent_ptr<CommandBuffer> cmdBuffer) noexcept
{
    VkDispatchIndirectCommand dispatch{x, y, z};
    setValue(dispatch, std::move(cmdBuffer));
}

VkDispatchIndirectCommand DispatchCountBuffer::getDispatch() const noexcept
{
    VkDispatchIndirectCommand dispatch = {};
    [[maybe_unused]] bool result = getValue(&dispatch);
    assert(result);
    return dispatch;
}
} // namespace magma
