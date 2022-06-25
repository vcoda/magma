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
#include "nondispatchable.h"

namespace magma
{
    class SamplerState;
    class DepthSamplerState;
    class BorderColor;

    /* Sampler objects represent the state of an image sampler
       which is used by the implementation to read image data
       and apply filtering and other transformations for the shader. */

    class Sampler : public NonDispatchable<VkSampler>
    {
    public:
        explicit Sampler(std::shared_ptr<Device> device,
            const SamplerState& state,
            std::shared_ptr<IAllocator> allocator = nullptr);
        explicit Sampler(std::shared_ptr<Device> device,
            const SamplerState& state,
            const BorderColor& borderColor,
            std::shared_ptr<IAllocator> allocator = nullptr);
        ~Sampler();

    protected:
        Sampler(std::shared_ptr<Device> device,
            std::shared_ptr<IAllocator> allocator):
            NonDispatchable(VK_OBJECT_TYPE_SAMPLER, std::move(device), std::move(allocator)) {}
    };

    /* Sampler with level of detail control. */

    class LodSampler : public Sampler
    {
    public:
        explicit LodSampler(std::shared_ptr<Device> device,
            const SamplerState& state,
            float mipLodBias,
            float minLod,
            float maxLod,
            const BorderColor& borderColor,
            std::shared_ptr<IAllocator> allocator = nullptr);
    };

    /* Depth map comparison sampler. Used to enable comparison against
       a reference value during lookups. */

    class DepthSampler : public Sampler
    {
    public:
        explicit DepthSampler(std::shared_ptr<Device> device,
            const DepthSamplerState& state,
            std::shared_ptr<IAllocator> allocator = nullptr);
    };

    /* Unnormalized sampler specifies the usage of unnormalized texel coordinates
       to address texels of the image. The range of the image coordinates used to
       lookup the texel is in the range of zero to the image dimensions for x, y and z. */

    class UnnormalizedSampler : public Sampler
    {
    public:
        explicit UnnormalizedSampler(std::shared_ptr<Device> device,
            bool linearFilter,
            std::shared_ptr<IAllocator> allocator = nullptr);
        explicit UnnormalizedSampler(std::shared_ptr<Device> device,
            bool linearFilter,
            const BorderColor& borderColor,
            std::shared_ptr<IAllocator> allocator = nullptr);
    };

    /* Reduction sampler allows to produce a filtered texel value by computing
       a component-wise minimum or maximum of the texels that would normally be averaged.
       https://www.khronos.org/registry/vulkan/specs/1.3-extensions/man/html/VK_EXT_sampler_filter_minmax.html */

#ifdef VK_EXT_sampler_filter_minmax
    class ReductionSampler : public Sampler
    {
    public:
        explicit ReductionSampler(std::shared_ptr<Device> device,
            const SamplerState& state,
            VkSamplerReductionModeEXT reductionMode,
            std::shared_ptr<IAllocator> allocator = nullptr);
        explicit ReductionSampler(std::shared_ptr<Device> device,
            const SamplerState& state,
            VkSamplerReductionModeEXT reductionMode,
            const BorderColor& borderColor,
            std::shared_ptr<IAllocator> allocator = nullptr);
    };
#endif // VK_EXT_sampler_filter_minmax
} // namespace magma
