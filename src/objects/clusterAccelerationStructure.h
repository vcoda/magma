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
#include "model/nondispatchable.h"

namespace magma
{
    class Buffer;

#ifdef VK_NV_cluster_acceleration_structure
    class ClusterAccelerationStructure
    {
    public:
        explicit ClusterAccelerationStructure(std::shared_ptr<Device> device);
        const std::unique_ptr<Buffer>& getImplicitData() const noexcept { return implicitData; }
        const std::unique_ptr<Buffer>& getAddressesArray() const noexcept { return addressesArrayBuffer; }
        const std::unique_ptr<Buffer>& getSizesArray() const noexcept { return sizesArrayBuffer; }
        const std::unique_ptr<Buffer>& getInfosArray() const noexcept { return infosArrayBuffer; }

    private:
        std::unique_ptr<Buffer> implicitData;
        std::unique_ptr<Buffer> addressesArrayBuffer;
        std::unique_ptr<Buffer> sizesArrayBuffer;
        std::unique_ptr<Buffer> infosArrayBuffer;
    };
#endif // VK_NV_cluster_acceleration_structure
} // namespace magma
