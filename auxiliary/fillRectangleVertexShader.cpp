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
#include "pch.h"
#pragma hdrstop
#include "fillRectangleVertexShader.h"
#include "../objects/device.h"
#include "../objects/shaderModule.h"
#include "../shaders/shaderReflection.h"
#include "../states/rasterizationState.h"

namespace magma
{
namespace aux
{
FillRectangleVertexShader::FillRectangleVertexShader(std::shared_ptr<Device> device,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
#ifdef VK_NV_fill_rectangle
    hasFillRectangle(device->extensionEnabled(VK_NV_FILL_RECTANGLE_EXTENSION_NAME))
#else
    hasFillRectangle(false)
#endif
{
    if (hasFillRectangle)
    {
        constexpr
        #include "spirv/output/blitv_nv"
        constexpr hash_t hash = core::hashArray(vsBlitNV);
        shader = std::make_shared<ShaderModule>(std::move(device), vsBlitNV, hash, std::move(allocator), 0, true);
    }
    else
    {
        constexpr
        #include "spirv/output/blitv"
        constexpr hash_t hash = core::hashArray(vsBlit);
        shader = std::make_shared<ShaderModule>(std::move(device), vsBlit, hash, std::move(allocator), 0, true);
    }
}

const char *FillRectangleVertexShader::getEntryPointName() const noexcept
{
    if (shader->getReflection())
        return shader->getReflection()->getEntryPointName(0);
    return "main";
}

const RasterizationState& FillRectangleVertexShader::getRasterizationState() const noexcept
{
#ifdef VK_NV_fill_rectangle
    if (hasFillRectangle)
        return renderstate::fillRectangleCullNoneCCw;
#endif
    return renderstate::fillCullNoneCCw;
}
} // namespace aux
} // namespace magma
