/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2019 Victor Coda.

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
    class Layers
    {
    public:
        bool hasLayer(const char *name) const noexcept;

    protected:
        Layers(const std::vector<VkLayerProperties>&);
        inline bool hasLayer(std::size_t hash) const noexcept
        {
            return layers.find(hash) != layers.end();
        }

    private:
        std::unordered_set<std::size_t> layers;
    };
} // namespace magma

#define MAGMA_LAYER(layer)\
    const bool layer;\
    static constexpr std::size_t __hash_of_ ## layer = core::hashString(MAGMA_CONCAT("VK_LAYER_", #layer));

#define MAGMA_CHECK_LAYER(layer) layer(hasLayer(__hash_of_ ## layer))
