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
template<typename Type>
MAGMA_INLINE SrcTransferBuffer::SrcTransferBuffer(std::shared_ptr<const Device> device,
    const std::vector<Type>& data,
    VkBufferCreateFlags flags /* 0 */,
    std::shared_ptr<IAllocator> allocator /* nullptr */,
    CopyMemoryFunction copyFn /* nullptr */):
    SrcTransferBuffer(device, data.data(), static_cast<VkDeviceSize>(data.size() * sizeof(Type)), flags,
        std::move(allocator), std::move(copyFn))
{}
} // namespace magma
