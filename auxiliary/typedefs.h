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
#pragma once

namespace magma
{
    namespace aux
    {
        typedef std::shared_ptr<class BaseFramebuffer> BaseFramebufferPtr;
        typedef std::shared_ptr<class BlitRectangle> BlitRectanglePtr;
        typedef std::shared_ptr<class Framebuffer> FramebufferPtr;
        typedef std::shared_ptr<class FrameGrabber> FrameGrabberPtr;
        typedef std::shared_ptr<class GraphicsPipelineCache> GraphicsPipelineCachePtr;
        typedef std::shared_ptr<class ImmediateRender> ImmediateRenderPtr;
        typedef std::shared_ptr<class IShaderIncludeHandler> ShaderIncludeHandlerPtr;
        typedef std::shared_ptr<class MipmapGenerator> MipmapGeneratorPtr;
        typedef std::shared_ptr<class MultisampleFramebuffer> MultisampleFramebufferPtr;
        typedef std::shared_ptr<class ShaderCompiler> ShaderCompilerPtr;
        typedef std::shared_ptr<class SwapchainFramebuffer> SwapchainFramebufferPtr;
    } // namespace aux
} // namespace magma
