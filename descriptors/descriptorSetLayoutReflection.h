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
       descriptor set layout. It contains a list of bindings to allow update their
       descriptors writes, validating for unique locations, check for dirty state etc. */

    class DescriptorSetLayoutReflection : core::NonCopyable
    {
    public:
        virtual const std::vector<descriptor::Descriptor*>& getBindingDescriptors() = 0;
        bool dirty();

    protected:
        template<class... Descriptor>
        void setReflection(Descriptor&&... args);
        const std::vector<descriptor::Descriptor*>& getReflection() const noexcept;
        bool hasReflection() const noexcept { return !reflection.empty(); }

    private:
        std::vector<descriptor::Descriptor*> reflection;
    };
} // namespace magma

#include "descriptorSetLayoutReflection.inl"
