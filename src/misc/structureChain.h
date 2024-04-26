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
    /* Binary data blob of Vulkan structure. */

    class ChainNode
    {
    public:
        template<class T>
        ChainNode(const T& n) noexcept:
            size(sizeof(T)), data(core::copyBinaryData(n))
        {
            static_assert(std::is_trivially_copyable<T>::value,
                "node type is required to be trivially copyable");
        }
        ChainNode(const ChainNode& n) noexcept: size(n.size),
            data(core::copyBinaryData(n.data, n.size)) {}
        ChainNode(ChainNode&& n) noexcept: size(n.size), data(n.data)
            { core::zeroMemory(n); }
        ~ChainNode() { delete[] data; }
        VkBaseOutStructure *getNode() noexcept
            { return reinterpret_cast<VkBaseOutStructure *>(data); }
        const VkBaseInStructure *getNode() const noexcept
            { return reinterpret_cast<const VkBaseInStructure *>(data); }
        std::size_t getSize() const noexcept { return size; }
        hash_t getHash() const noexcept
            { return core::hashArray(data, size); }

    private:
        size_t size;
        uint8_t *data;
    };

    /* Vulkan structures that are chained into linked list with head
       node assigned to the pNext member of Vk*CreateInfo structure. */

    class StructureChain
    {
    public:
        template<class StructureType>
        void linkNode(const StructureType& node)
            { insertNode(node); }
        template<class StructureType>
        StructureType *findNode() noexcept;
        template<class StructureType>
        const StructureType *findNode() const noexcept;
        VkBaseOutStructure *firstNode() noexcept
            { return chain.empty() ? nullptr : chain.begin()->getNode(); }
        const VkBaseInStructure *firstNode() const noexcept
            { return chain.empty() ? nullptr : chain.cbegin()->getNode(); }
        VkBaseOutStructure *lastNode() noexcept
            { return chain.empty() ? nullptr : chain.rbegin()->getNode(); }
        const VkBaseInStructure *lastNode() const noexcept
            { return chain.empty() ? nullptr : chain.crbegin()->getNode(); }
        uint32_t getSize() const noexcept { return MAGMA_COUNT(chain); }
        bool empty() const noexcept { return chain.empty(); }
        VkBaseOutStructure *getChain() noexcept
            { return chain.empty() ? nullptr : chain.begin()->getNode(); }
        const VkBaseInStructure *getChain() const noexcept
            { return chain.empty() ? nullptr : chain.cbegin()->getNode(); }
        hash_t getHash() const noexcept;

    private:
        void insertNode(const ChainNode& node);
        VkBaseOutStructure *lookupNode(VkStructureType sType) noexcept;
        const VkBaseInStructure *lookupNode(VkStructureType sType) const noexcept;

        std::vector<ChainNode> chain;
    };
} // namespace magma

#define MAGMA_SPECIALIZE_STRUCTURE_CHAIN_NODE(StructureType, structureType)\
template<>\
inline StructureType *magma::StructureChain::findNode<StructureType>() noexcept\
{\
    return reinterpret_cast<StructureType *>(lookupNode(structureType));\
}\
\
template<>\
inline const StructureType *magma::StructureChain::findNode<StructureType>() const noexcept\
{\
    return reinterpret_cast<const StructureType *>(lookupNode(structureType));\
}

#include "structureChainNode.inl"
