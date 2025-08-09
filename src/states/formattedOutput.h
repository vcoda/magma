/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2025 Victor Coda.

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
    class VertexInputState;
    struct InputAssemblyState;
    struct TesselationState;
    class ViewportState;
    struct RasterizationState;
    struct MultisampleState;
    struct DepthStencilState;
    struct ColorBlendState;
} // namespace magma

std::ostream& operator<<(std::ostream&, const magma::VertexInputState&);
std::ostream& operator<<(std::ostream&, const magma::InputAssemblyState&);
std::ostream& operator<<(std::ostream&, const magma::TesselationState&);
std::ostream& operator<<(std::ostream&, const magma::ViewportState&);
std::ostream& operator<<(std::ostream&, const magma::RasterizationState&);
std::ostream& operator<<(std::ostream&, const magma::MultisampleState&);
std::ostream& operator<<(std::ostream&, const magma::DepthStencilState&);
std::ostream& operator<<(std::ostream&, const magma::ColorBlendState&);
