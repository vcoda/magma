/*
Magma - Abstraction layer over Khronos Vulkan API.
Copyright (C) 2018-2024 Victor Coda.

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
#include "compatibility.h"

namespace magma
{
    /* A chain of Vulkan structures that is implemented
       as singly-linked list. No extra storage is required.
       Instance of this class is copyable and movable. */

    class StructureChain final
    {
    public:
        StructureChain() noexcept;
        template<class StructureType>
        StructureChain(const StructureType& node) noexcept;
        StructureChain(const StructureChain& other) noexcept;
        StructureChain(StructureChain&& other) noexcept;
        ~StructureChain() { clear(); }
        template<class StructureType>
        void linkNode(const StructureType& node) noexcept;
        template<class StructureType>
        StructureType *findNode() noexcept;
        template<class StructureType>
        const StructureType *findNode() const noexcept;
        VkBaseOutStructure *headNode() noexcept { return head; }
        const VkBaseInStructure *headNode() const noexcept
            { return reinterpret_cast<const VkBaseInStructure *>(head); }
        VkBaseOutStructure *tailNode() noexcept;
        const VkBaseInStructure *tailNode() const noexcept;
        size_t size() const noexcept;
        void clear() noexcept;
        bool empty() const noexcept { return !head; }
        hash_t hash() const noexcept;

    private:
        VkBaseOutStructure *getNode(VkStructureType) noexcept;
        const VkBaseInStructure *getNode(VkStructureType) const noexcept;
        hash_t getNodeHash(VkBaseOutStructure *node) const noexcept;
        static VkBaseOutStructure *copyNode(const VkBaseOutStructure *node) noexcept;
        static size_t getNodeSize(VkStructureType) noexcept;

        VkBaseOutStructure *head;
    };
} // namespace magma

#include "structureChain.inl"
#include "structureChainNode.inl"
