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
    /* Vulkan structures that are chained into linked list with head
       node assigned to the pNext member of Vk*CreateInfo structure. */

    class StructureChain
    {
    public:
        template<class StructureType>
        void addNode(const StructureType& node);
        template<class StructureType>
        StructureType *findNode() const noexcept;
        VkBaseOutStructure *firstNode() noexcept;
        const VkBaseInStructure *firstNode() const noexcept;
        VkBaseOutStructure *lastNode() noexcept;
        const VkBaseInStructure *lastNode() const noexcept;
        uint32_t getSize() const noexcept { return MAGMA_COUNT(chain); }
        bool empty() const noexcept { return chain.empty(); }
        VkBaseOutStructure *chainNodes() noexcept;
        const VkBaseInStructure *chainNodes() const noexcept;
        hash_t getHash() const noexcept;

    private:
        class Node;
        mutable std::list<Node> chain;
    };
} // namespace magma

#define MAGMA_SPECIALIZE_STRUCTURE_CHAIN_NODE(StructureType, structureType)\
template<>\
inline StructureType *magma::StructureChain::findNode<StructureType>() const noexcept\
{\
    auto it = std::find_if(chain.begin(), chain.end(),\
        [](auto& it)\
        {\
           return (it.getNode()->sType == structureType);\
        });\
    if (it != chain.end())\
        return reinterpret_cast<StructureType *>(it->getNode());\
    return nullptr;\
}

#include "structureChain.inl"
#include "structureChainNode.inl"
