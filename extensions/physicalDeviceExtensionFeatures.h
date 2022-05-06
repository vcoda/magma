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
        VkBool32 deviceCoherentMemory = 0;
        // EXT
        VkBool32 advancedBlendCoherentOperations = 0;
        VkBool32 depthClipControl = 0;
        VkBool32 depthClipEnable = 0;
        VkBool32 extendedDynamicState = 0;
        VkBool32 hostQueryReset = 0;
        VkBool32 robustImageAccess = 0;
        VkBool32 indexTypeUint8 = 0;
        VkBool32 memoryPriority = 0;
        VkBool32 pipelineCreationCacheControl = 0;
        VkBool32 privateData = 0;
        VkBool32 scalarBlockLayout = 0;
        VkBool32 shaderDemoteToHelperInvocation = 0;
        VkBool32 texelBufferAlignment = 0;
        VkBool32 ycbcrImageArrays = 0;
        // KHR
        VkBool32 imagelessFramebuffer = 0;
        VkBool32 pipelineExecutableInfo = 0;
        VkBool32 samplerYcbcrConversion = 0;
        VkBool32 separateDepthStencilLayouts = 0;
        VkBool32 shaderDrawParameters = 0;
        VkBool32 shaderSubgroupExtendedTypes = 0;
        VkBool32 shaderTerminateInvocation = 0;
        VkBool32 timelineSemaphore = 0;
        VkBool32 uniformBufferStandardLayout = 0;
        // NV
        VkBool32 cornerSampledImage = 0;
        VkBool32 coverageReductionMode = 0;
        VkBool32 dedicatedAllocationImageAliasing = 0;
        VkBool32 diagnosticsConfig = 0;
        VkBool32 fragmentShaderBarycentric = 0;
        VkBool32 representativeFragmentTest = 0;
        VkBool32 exclusiveScissor = 0;
        VkBool32 imageFootprint = 0;
        VkBool32 shaderSMBuiltins = 0;
    };
} // namespace magma
