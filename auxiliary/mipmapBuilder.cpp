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
#include "mipmapBuilder.h"
#include "../objects/device.h"
#include "../objects/image1D.h"
#include "../objects/image2D.h"
#include "../objects/commandBuffer.h"

namespace magma
{
namespace aux
{
MipmapBuilder::MipmapBuilder(std::shared_ptr<Device> device, std::shared_ptr<IAllocator> allocator /* nullptr */)
{
}

void MipmapBuilder::buildMipmap1D(std::shared_ptr<Image1D> baseLevel, std::shared_ptr<CommandBuffer> cmdBuffer) const noexcept
{
}

void MipmapBuilder::buildMipmap2D(std::shared_ptr<Image2D> baseLevel, std::shared_ptr<CommandBuffer> cmdBuffer) const noexcept
{
}
} // namespace aux
} // namespace magma
