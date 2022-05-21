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
    namespace aux
    {
        MAGMA_TYPEDEF_SHARED_PTR(AccumulationBuffer)
        MAGMA_TYPEDEF_SHARED_PTR(BaseFramebuffer)
        MAGMA_TYPEDEF_SHARED_PTR(BlitRectangle)
        MAGMA_TYPEDEF_SHARED_PTR(ColorFramebuffer)
        MAGMA_TYPEDEF_SHARED_PTR(ColorMultisampleFramebuffer)
        MAGMA_TYPEDEF_SHARED_PTR(ColorMultisampleReadFramebuffer)
        MAGMA_TYPEDEF_SHARED_PTR(DepthFramebuffer);
        MAGMA_TYPEDEF_SHARED_PTR(FillRectangleVertexShader)
        MAGMA_TYPEDEF_SHARED_PTR(FrameGrabber)
        MAGMA_TYPEDEF_SHARED_PTR(GraphicsPipelineCache)
        MAGMA_TYPEDEF_SHARED_PTR(ImmediateRender)
        MAGMA_TYPEDEF_SHARED_PTR(MipmapGenerator)
        MAGMA_TYPEDEF_SHARED_PTR(MultiAttachmentFramebuffer)
        MAGMA_TYPEDEF_SHARED_PTR(ShaderCompiler)
        MAGMA_TYPEDEF_SHARED_PTR(Sprite)
        MAGMA_TYPEDEF_SHARED_PTR(SwapchainFramebuffer)
        MAGMA_TYPEDEF_SHARED_PTR(TextShader)

        MAGMA_TYPEDEF_INTERFACE_SHARED_PTR(ShaderIncludeHandler)
    } // namespace aux
} // namespace magma
