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
#include "imageView.h"

namespace magma
{
    /* By default, the fragment density map is locked by the host
       for reading between vkCmdBeginRenderPass during recording and
       VK_PIPELINE_STAGE_FRAGMENT_DENSITY_PROCESS_BIT_EXT during draw execution.
       This can introduce large latency for certain use cases between recording
       the frame and displaying the frame. Apps may wish to modify
       the fragment density map just before draw execution.

       If VK_IMAGE_VIEW_CREATE_FRAGMENT_DENSITY_MAP_DYNAMIC_BIT_EXT is used,
       then the application can modify the image until the device reads it
       during VK_PIPELINE_STAGE_FRAGMENT_DENSITY_PROCESS_BIT_EXT.

       VK_IMAGE_VIEW_CREATE_FRAGMENT_DENSITY_MAP_DEFERRED_BIT_EXT is intended
       to help address this for implementations that do not support the
       fragmentDensityMapDynamic feature by deferring the start of the
       locked range to vkEndCommandBuffer. */

#ifdef VK_EXT_fragment_density_map
    class FragmentDensityMap;

    class FragmentDensityMapView : public ImageView
    {
    public:
        explicit FragmentDensityMapView(std::shared_ptr<FragmentDensityMap> fragmentDensityMap,
            bool fragmentDensityMapDynamic,
            bool fragmentDensityMapDeferred);
        bool dynamic() const noexcept { return flags & VK_IMAGE_VIEW_CREATE_FRAGMENT_DENSITY_MAP_DYNAMIC_BIT_EXT; }
    #ifdef VK_EXT_fragment_density_map2
        bool deferred() const noexcept { return flags & VK_IMAGE_VIEW_CREATE_FRAGMENT_DENSITY_MAP_DEFERRED_BIT_EXT; }
    #endif
    };
#endif // VK_EXT_fragment_density_map
} // namespace magma
