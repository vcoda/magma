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
#include "fillRectangleVertexShader.h"
#include "../objects/device.h"
#include "../objects/shaderModule.h"
#include "../states/rasterizationState.h"

#ifndef VK_NV_FILL_RECTANGLE_EXTENSION_NAME
#define VK_NV_FILL_RECTANGLE_EXTENSION_NAME "VK_NV_fill_rectangle"
#endif

namespace magma::aux
{
FillRectangleVertexShader::FillRectangleVertexShader(std::shared_ptr<Device> device,
    std::shared_ptr<IAllocator> allocator /* nullptr */):
    PipelineShaderStage(VK_SHADER_STAGE_VERTEX_BIT,
        [&]() -> std::shared_ptr<ShaderModule>
        {
            constexpr
            #include "spirv/output/blitv"
            constexpr
            #include "spirv/output/blitv_nv"
            constexpr hash_t hash = core::hashArray(vsBlit);
            constexpr hash_t hashNV = core::hashArray(vsBlitNV);
            const bool NV_fill_rectangle = device->extensionEnabled(VK_NV_FILL_RECTANGLE_EXTENSION_NAME);
            return std::make_shared<ShaderModule>(device,
                NV_fill_rectangle ? vsBlitNV : vsBlit,
                NV_fill_rectangle ? sizeof(vsBlitNV) : sizeof(vsBlit),
                NV_fill_rectangle ? hashNV : hash,
                std::move(allocator));
        }(),
        "main"),
    rasterizationState(device->extensionEnabled(VK_NV_FILL_RECTANGLE_EXTENSION_NAME)
        ? renderstate::fillRectangleCullNoneCcw
        : renderstate::fillCullNoneCcw)
{}
} // namespace magma::aux
