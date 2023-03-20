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
#include "descriptor.h"

namespace magma
{
    typedef std::vector<std::reference_wrapper<descriptor::Descriptor>> DescriptorList;

    /* Provides a reflection mechanism making it possible to investigate members of
       descriptor set layout. Table contains an array of its descriptors to perform validation,
       check dirty states, update their descriptors writes etc.

       Usage example:

       struct MyDescriptorTable : magma::DescriptorSetTable
       {
            magma::descriptor::UniformBuffer worldViewProj = 0;
            magma::descriptor::CombinedImageSampler albedo = 1;
            magma::descriptor::CombinedImageSampler normal = 2;
            MAGMA_REFLECT(worldViewProj, albedo, normal)
       };
    */

    class DescriptorSetTable : core::NonCopyable
    {
    public:
        virtual const DescriptorList& getReflection() = 0;
        std::size_t getSize() { return getReflection().size(); }
        bool dirty();

    protected:
        template<class... Descriptor>
        inline void setReflection(Descriptor&&... args);

        DescriptorList reflection;
    };
} // namespace magma

#include "descriptorSetTable.inl"
