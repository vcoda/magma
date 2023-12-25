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

namespace magma
{
    /* For each layer name we calculate its hash in compile-
       time. When layers are enumerated, their hashes are
       added to hash map, which allows a fast lookup of a layer. */

    class Layers
    {
    public:
        bool hasLayer(const char *name) const noexcept;
        void forEach(std::function<void(const VkLayerProperties&)> fn) const noexcept;

    protected:
        Layers(const std::vector<VkLayerProperties>&);
        bool hasLayer(hash_t hash) const noexcept;

    private:
        std::map<std::string, VkLayerProperties> layers;
        std::unordered_set<hash_t> hashes;
    };

    MAGMA_TYPEDEF_UNIQUE_PTR(Layers);
} // namespace magma
