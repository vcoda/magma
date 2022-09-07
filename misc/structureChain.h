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
    /* Vulkan structures that are chained into linked list with head node
       assigned to the pNext member of Vk*CreateInfo structure. */

    class StructureChain
    {
    public:
        template<class Structure>
        void addNode(const Structure& node);
        const void *getChainedNodes() const noexcept;
        hash_t getHash() const noexcept;
        std::size_t numNodes() const noexcept { return chain.size(); }
        bool isEmpty() const noexcept { return chain.empty(); }

    private:
        struct Node
        {
            template<class Structure>
            Node(const Structure& node);
            VkBaseOutStructure *getNode() noexcept;
            const hash_t hash;
            std::vector<uint8_t> blob;
        };

        mutable std::list<Node> chain;
    };
} // namespace magma

#include "structureChain.inl"
