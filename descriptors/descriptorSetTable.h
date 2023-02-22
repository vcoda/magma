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
    /* Provides a reflection mechanism making it possible to investigate members of
       descriptor set layout. Table contains an array of its descriptors to perform validation,
       check dirty states, update their descriptors writes etc.

       Usage example:

       struct MyDescriptorTable : magma::DescriptorSetTable
       {
            magma::descriptor::UniformBuffer worldViewProj = 0;
            magma::descriptor::CombinedImageSampler albedo = 1;
            magma::descriptor::CombinedImageSampler normal = 2;
            MAGMA_REFLECT(&worldViewProj, &albedo, &normal)
       }; */

    class DescriptorSetTable : core::NonCopyable
    {
    public:
        virtual const std::vector<descriptor::Descriptor*>& getReflection() = 0;

        bool dirty()
        {
            for (auto descriptor: reflection)
            {
                if (descriptor->dirty())
                    return true;
            }
            return false;
        }

    protected:
        template<class... Descriptor>
        inline void setReflection(Descriptor&&... args)
        {   // Use "temporary array" idiom
            // https://stackoverflow.com/questions/28866559/writing-variadic-template-constructor
            std::initializer_list<int>{
                (reflection.push_back(std::forward<Descriptor>(args)), void(), 0)...
            };
        }

        std::vector<descriptor::Descriptor*> reflection;
    };
} // namespace magma

/* Variadic macro used to simplify reflection of layout structure members.
   It takes a variable number of arguments through __VA_ARGS__ and uses
   variadic template method to populate the list of descriptor set bindings. */

#define MAGMA_REFLECT(...)\
const std::vector<magma::descriptor::Descriptor*>& getReflection() override\
{\
    if (reflection.empty())\
        setReflection(__VA_ARGS__);\
    return reflection;\
}
