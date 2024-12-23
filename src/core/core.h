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
#include "constants.h"
#include "defines.h"
#include "macros.h"
#include "variantPtr.h"
#include "lentPtr.h"
#include "vla.h"
#include "hashing/constexpr.h"
#include "hashing/fnv1.h"
#include "hashing/fnv1string.h"

namespace magma
{
    typedef std::vector<const char *> NullTerminatedStringArray;

#if (VK_USE_64_BIT_PTR_DEFINES == 1)
    typedef void *NonDispatchableHandle;
#else
    typedef uint64_t NonDispatchableHandle;
#endif

    class IClass
    {
    public:
        virtual ~IClass() = default;
    };

    class NonCopyable
    {
        NonCopyable(const NonCopyable&) = delete;
        const NonCopyable& operator=(const NonCopyable&) = delete;

    protected:
        NonCopyable() = default;
        ~NonCopyable() = default;
    };

    template<class Parent, class Child>
    inline void linkNode(Parent& parent, Child& child) noexcept
    {
        child.pNext = const_cast<void *>(parent.pNext);
        parent.pNext = &child;
    }

    namespace core
    {
        template<class T> constexpr bool powerOfTwo(T x) noexcept;
        template<class T> constexpr T alignUp(T value, T alignment) noexcept;
        template<class T> constexpr T alignDown(T value, T alignment) noexcept;
        template<class T> constexpr T roundUp(T value, T multiple) noexcept;

        template<class T> uint32_t countof(const T& container) noexcept;
        template<class T> typename T::NativeHandle dereference(T *p) noexcept;
        template<class VkObject> VkObject reinterpret(NonDispatchableHandle handle) noexcept;

        template<class T1, class T2, class L> void foreach(T1& container1, T2& container2, L&& fn);

        template<class T> constexpr hash_t hash(T x) noexcept;
        template<class T> constexpr hash_t hash(T *p) noexcept;
        template<class T> constexpr hash_t hashArg(hash_t seed, const T& arg) noexcept;
        template<class T, typename... Args> constexpr hash_t hashArgs(const T& arg, Args... args) noexcept;
        template<class T, std::size_t N> constexpr hash_t hashArray(const T (&arr)[N]) noexcept;
        template<class T> hash_t hashArray(const T arr[], std::size_t count) noexcept;
        template<class T> constexpr hash_t hashString(const T *str) noexcept;
        template<class T> hash_t hashString(const std::basic_string<T>& str) noexcept;
        hash_t combineHashList(const std::initializer_list<hash_t>& hashes) noexcept;

        template<class T> T *copy(const T *src) noexcept;
        template<class T> T *copy(T *dst, const T *src) noexcept;
        template<class T> T *copyArray(const T *src, std::size_t size) noexcept;
        template<class T> T *copyVector(const std::vector<T>& src) noexcept;
        template<class T1, class T2> T1 *copyVector(const std::vector<T2>& src) noexcept;
        template<class T> T *copyInitializerList(const std::initializer_list<T>& src) noexcept;
        template<class T1, class T2> T1 *copyInitializerList(const std::initializer_list<T2>& src) noexcept;
        char *copyString(const char *src) noexcept;
        char *copyString(char *dst, std::size_t size, const char *src) noexcept;
        uint8_t *copyBinaryData(const void *src, std::size_t size) noexcept;
        template<class T> uint8_t *copyBinaryData(const T& src) noexcept;
        template<class T> bool compare(const void *p1, const void *p2) noexcept;
        template<class T> bool compare(const T *p1, const T *p2) noexcept;
        template<class T> bool compareArrays(const T *p1, const T *p2, std::size_t count) noexcept;
        template<class T> void zeroMemory(T& var) noexcept;
        template<class T, std::size_t N> void zeroMemory(T (&array)[N]) noexcept;
    } // namespace core
} // namespace magma

#include "core.inl"
#include "hash.inl"
#include "memory.inl"
