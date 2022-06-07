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
#include "extensions.h"

namespace magma
{
    struct PhysicalDeviceExtensionFeatures
    {   // AMD
        VkBool32 deviceCoherentMemory : 1;
        // EXT
        VkBool32 advancedBlendCoherentOperations : 1;
        VkBool32 depthClipControl : 1;
        VkBool32 depthClipEnable : 1;
        VkBool32 extendedDynamicState : 1;
        VkBool32 hostQueryReset : 1;
        VkBool32 robustImageAccess : 1;
        VkBool32 indexTypeUint8 : 1;
        VkBool32 memoryPriority : 1;
        VkBool32 pipelineCreationCacheControl : 1;
        VkBool32 privateData : 1;
        VkBool32 scalarBlockLayout : 1;
        VkBool32 shaderDemoteToHelperInvocation : 1;
        VkBool32 texelBufferAlignment : 1;
        VkBool32 ycbcrImageArrays : 1;
        // KHR
        VkBool32 imagelessFramebuffer : 1;
        VkBool32 pipelineExecutableInfo : 1;
        VkBool32 samplerYcbcrConversion : 1;
        VkBool32 separateDepthStencilLayouts : 1;
        VkBool32 shaderDrawParameters : 1;
        VkBool32 shaderSubgroupExtendedTypes : 1;
        VkBool32 shaderTerminateInvocation : 1;
        VkBool32 timelineSemaphore : 1;
        VkBool32 uniformBufferStandardLayout : 1;
        // NV
        VkBool32 cornerSampledImage : 1;
        VkBool32 coverageReductionMode : 1;
        VkBool32 dedicatedAllocationImageAliasing : 1;
        VkBool32 diagnosticsConfig : 1;
        VkBool32 fragmentShaderBarycentric : 1;
        VkBool32 representativeFragmentTest : 1;
        VkBool32 exclusiveScissor : 1;
        VkBool32 imageFootprint : 1;
        VkBool32 shaderSMBuiltins : 1;
    };
} // namespace magma
