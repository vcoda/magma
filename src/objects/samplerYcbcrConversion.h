/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2023 Victor Coda.

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
#include "model/nondispatchable.h"

namespace magma
{
    struct SamplerYcbcrConversionState;

    /* Provides the ability to perform specified color space
       conversions during texture sampling operations for the
       YCbCr color space natively. */

#ifdef VK_KHR_sampler_ycbcr_conversion
    class SamplerYcbcrConversion : public NonDispatchable<VkSamplerYcbcrConversionKHR>
    {
    public:
        explicit SamplerYcbcrConversion(std::shared_ptr<Device> device,
            VkFormat format,
            const SamplerYcbcrConversionState& state,
            std::shared_ptr<IAllocator> allocator = nullptr,
            const VkComponentMapping& swizzle = {
                VK_COMPONENT_SWIZZLE_IDENTITY,
                VK_COMPONENT_SWIZZLE_IDENTITY,
                VK_COMPONENT_SWIZZLE_IDENTITY,
                VK_COMPONENT_SWIZZLE_IDENTITY},
            VkChromaLocationKHR xChromaOffset = VK_CHROMA_LOCATION_MIDPOINT_KHR,
            VkChromaLocationKHR yChromaOffset = VK_CHROMA_LOCATION_MIDPOINT_KHR,
            bool forceExplicitReconstruction = false,
            const StructureChain& extendedInfo = StructureChain());
        ~SamplerYcbcrConversion();
    };
#endif // VK_KHR_sampler_ycbcr_conversion
} // namespace magma
