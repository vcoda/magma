/*
Magma - abstraction layer to facilitate usage of Khronos Vulkan API.
Copyright (C) 2018-2021 Victor Coda.

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
namespace core
{
/* Iterates two containers simultaneously. */

template<class First, class Second>
inline void forEach(First& a, Second& b,
    std::function<void(typename First::iterator&,
                       typename Second::iterator&)> eachFn)
{
    typename First::iterator i;
    typename Second::iterator j;
    for (i = a.begin(), j = b.begin();
        (i != a.end()) && (j != b.end());
        ++i, ++j)
    {
        eachFn(i, j);
    }
}

/* Iterates two immutable containers simultaneously. */

template<class First, class Second>
inline void forConstEach(const First& a, const Second& b,
    std::function<void(typename First::const_iterator&,
                       typename Second::const_iterator&)> eachFn)
{
    typename First::const_iterator i;
    typename Second::const_iterator j;
    for (i = a.cbegin(), j = b.cbegin();
        (i != a.cend()) && (j != b.cend());
        ++i, ++j)
    {
        eachFn(i, j);
    }
}
} // namespace core
} // namespace magma
