/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2020 Victor Coda.

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
#include "baseFramebuffer.h"
#include "../objects/framebuffer.h"

namespace magma
{
namespace aux
{
Framebuffer::Framebuffer(uint32_t sampleCount) noexcept:
    sampleCount(sampleCount)
{}

const VkExtent2D& Framebuffer::getExtent() const noexcept
{
    return framebuffer->getExtent();
}

uint32_t Framebuffer::getSampleCount() const noexcept
{
    return sampleCount;
}

std::shared_ptr<RenderPass> Framebuffer::getRenderPass() noexcept
{
    return renderPass;
}

std::shared_ptr<const RenderPass> Framebuffer::getRenderPass() const noexcept
{
    return renderPass;
}

std::shared_ptr<magma::Framebuffer> Framebuffer::getFramebuffer() noexcept
{
    return framebuffer;
}

std::shared_ptr<const magma::Framebuffer> Framebuffer::getFramebuffer() const noexcept
{
    return framebuffer;
}
} // namespace aux
} // namespace magma
