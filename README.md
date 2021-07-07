# Abstraction layer to facilitate usage of Khronos Vulkan API

<p align="center">
    <img src="resources/vulkan.gif" width="94px" height="107px">
</p>

## Problem

Writing rendering stuff in Vulkan requires a lot of boilerplate code, unlike OpenGL and Direct3D 9.
This is the cost we pay for low-level and high-performant API, where we can control a lot of things
that previously only graphics driver was responsible for. Vulkan has very verbose interface where 
you have to initialize a lot of structures to get things work. This complicates fast prototyping,
makes your program error prone and hard to read and understand.

## Solutions

There are a lot of different C++ bindings, developed to simplify usage of Vulkan, including:

* [Vulkan-Hpp](https://github.com/KhronosGroup/Vulkan-Hpp)<br>
* [Anvil](https://github.com/GPUOpen-LibrariesAndSDKs/Anvil)<br>
* [V-EZ](https://github.com/GPUOpen-LibrariesAndSDKs/V-EZ)<br>
* [Vookoo](https://github.com/andy-thomason/Vookoo)<br>

and many others, but I've found that no one suitable for my purposes, because their implementations are 
different from my vision how such wrapper should be implemented and used. So eventually I decided to write my own.

## Design

Magma is all about initialization. It was designed with simplicity of object construction in mind.
Initialization exploit C++ [RAII](https://en.wikipedia.org/wiki/Resource_acquisition_is_initialization) idiom with constructor overloading and default parameters that can be omitted.
Also library has a lot of pre-defined state objects, so developer can use them without initialization of custom states. 
With this approach construction of the most complex Vulkan object - VkPipeline - takes only a dozen lines of code.

All objects are inherited from two types: Dispatchable and NonDispatchable, according to API specification. These types in turn
are inherited from base Object class. Render state objects are simply structures that inherited from Vulkan structures
and has plenty of constructors to conveniently initialize state description. They don't have any additional members,
so user can safely cast an array of state objects to an array of Vulkan structures. All state objects have ::hash() method,
which can be used to quickly lookup similar pipeline states in the cache instead of creating a new one.

The library was designed with zero or almost zero overhead in mind. While C++ exceptions are heavily used during object
construction time, there are numerous methods marked with "noexcept" specifier. Consider, for example, VkCommandBuffer object:

* Most of wrapper's methods don't throw any exceptions.
* Parameters like shared pointers are passed by reference, so there is no reference counter increment/decrement.
* Thin methods around API calls are made inline. 

Command buffer's calls in release build should be as much efficient as native C API calls.

Predefined render states are usually "constexpr" objects, which means that they are initialized at compile-time
(not run-time), mapping efficiently to low-level API.

The library often allocates temporary arrays on the stack instead of creating them in the heap. This may cause stack overflow
in abuse cases, but speeds up allocations and reduces memory fragmentation in run-time.

## Reflection

Defining descriptor set layouts usually the most complicated part of Vulkan API and often results in a lot of entagled code.
From conceptual point of view, it should be done using reflection between C++ and GLSL code. Reflection is a mechanism 
making it possible to investigate yourself. In programming languages, reflection is used to investigate format of objects at runtime,
invoke methods and access fields of these objects. While C++ doesn't support true reflection, Magma is trying to mimic it using variadics 
and SPIR-V reflection data. This allows to define descriptor set layouts as regular C++ structures and check their validity against 
specific shader bytecode. At run-time, you can assign resources (images, buffers, uniforms) to descriptor bindings and they will be 
attached to shader when descriptor set is binded using command buffer.

## Memory management

While it is legal to create buffers and images without custom memory allocator, it is highly encouraging to provide
host and device memory allocators during resource creation. Keep in mind that there is an implementation-dependent
limit of a number of memory allocations reported by VkPhysicalDeviceLimits::maxMemoryAllocationCount, so per-object allocation
approach suitable only for a small graphics applications. Custom allocator allows sub-allocations from larger memory chunks
and reduces memory fragmentation in dynamic scenarios. Magma provides IDeviceMemoryAllocator interface that allows you 
to implement your own allocator or you can use a default one which is written on top of [Vulkan Memory Allocator](https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator).
Usually "allocator" parameter is the first among default parameters passed in the constructor which simplifies construction expressions.

## Features

Magma was written mainly around [Vulkan 1.0](https://renderdoc.org/vkspec_chunked/index.html) specification, but has built-in support for some extensions,
like NVidia ray-tracing, fill rectangle primitive and some other minor extensions. There are a lot of mobile hardware that support only Vulkan 1.0,
so it is better to have a wider range of compatible hardware rather than a questionable features. Support for new API versions (1.1 and beyond) still not implemented.

## Auxiliary

Magma provides some auxiliary objects that were written on top of core functionality. Their goals are to facilitate development by providing
typical operations used in 3D graphics. For example, sometimes I miss immediate mode from OpenGL 1.x era, where you can quickly draw 
a few primitives with glBegin/glEnd. Or often there is need to quickly present rendered image to the screen, or create shader from GLSL source
instead of pre-compiled SPIR-V binary and so on. Auxiliary objects are placed in nested namespace and are not part of the Magma core.

Some auxiliary objects use built-in precompiled shaders. Hash of a bytecode of these shaders calculated in compile-time to reduce loading overhead.

## Dependencies

There are dependencies from the following third party libraries:

* [shaderc](https://github.com/google/shaderc) - a collection of tools, libraries, and tests for Vulkan shader compilation.
* [SPIRV-Reflect](https://github.com/chaoticbob/SPIRV-Reflect) - a lightweight library that provides a C/C++ reflection API for SPIR-V shader bytecode in Vulkan applications.
* [Vulkan Memory Allocator](https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator) - Vulkan memory allocation library.

Library depends on STL and has not been designed to be used with custom containers. It doesn't use any file input/output.

## Language

Code is written in modern C++11/14 and takes advantage of new language features.

Supported compilers:

* MSC
* GCC
