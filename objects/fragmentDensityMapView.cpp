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
#include "fragmentDensityMapView.h"
#include "fragmentDensityMap.h"

namespace magma
{
#ifdef VK_EXT_fragment_density_map
FragmentDensityMapView::FragmentDensityMapView(std::shared_ptr<FragmentDensityMap> fragmentDensityMap,
    bool fragmentDensityMapDynamic, bool fragmentDensityMapDeferred):
    ImageView(std::move(fragmentDensityMap),
        {VK_COMPONENT_SWIZZLE_IDENTITY,
         VK_COMPONENT_SWIZZLE_IDENTITY,
         VK_COMPONENT_SWIZZLE_IDENTITY,
         VK_COMPONENT_SWIZZLE_IDENTITY},
        (fragmentDensityMapDynamic ? VK_IMAGE_VIEW_CREATE_FRAGMENT_DENSITY_MAP_DYNAMIC_BIT_EXT : 0)
    #ifdef VK_EXT_fragment_density_map2
        | (fragmentDensityMapDeferred ? VK_IMAGE_VIEW_CREATE_FRAGMENT_DENSITY_MAP_DEFERRED_BIT_EXT : 0)
    #endif
        )
{}
#endif // VK_EXT_fragment_density_map
} // namespace magma
