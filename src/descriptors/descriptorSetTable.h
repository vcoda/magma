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
#include "descriptorSetLayoutBinding.h"

namespace magma
{
    typedef std::vector<std::reference_wrapper<DescriptorSetLayoutBinding>> DescriptorSetTableBindings;

    /* Provides a reflection mechanism making it possible to
       investigate members of descriptor set layout. Table
       contains an array of its descriptors to perform validation,
       check dirty states, update their descriptors writes etc.

       Usage example:

       struct MyDescriptorTable : magma::DescriptorSetTable
       {
            magma::descriptor::UniformBuffer worldViewProj = 0;
            magma::descriptor::CombinedImageSampler albedo = 1;
            magma::descriptor::CombinedImageSampler normal = 2;
            MAGMA_REFLECT(worldViewProj, albedo, normal)
       }; */

    class DescriptorSetTable : NonCopyable
    {
    public:
        virtual const DescriptorSetTableBindings& getReflection() noexcept = 0;
        std::size_t getSize() noexcept { return getReflection().size(); }
        bool valid() noexcept;
        bool dirty() noexcept;

    protected:
        template<class... Args>
        inline void setReflection(Args&&... args) noexcept;

        DescriptorSetTableBindings reflection;
    };
} // namespace magma

#include "descriptorSetTable.inl"
