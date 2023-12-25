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
namespace core
{
template<class Foo, class Bar>
inline void forEach(Foo& a, Bar& b,
    std::function<void(typename Foo::iterator&,
                       typename Bar::iterator&)> each)
{
    typename Foo::iterator i;
    typename Bar::iterator j;
    for (i = a.begin(), j = b.begin();
        (i != a.end()) && (j != b.end());
        ++i, ++j)
    {
        each(i, j);
    }
}

template<class Foo, class Bar>
inline void forConstEach(const Foo& a, const Bar& b,
    std::function<void(typename Foo::const_iterator&,
                       typename Bar::const_iterator&)> each)
{
    typename Foo::const_iterator i;
    typename Bar::const_iterator j;
    for (i = a.cbegin(), j = b.cbegin();
        (i != a.cend()) && (j != b.cend());
        ++i, ++j)
    {
        each(i, j);
    }
}
} // namespace core
} // namespace magma
