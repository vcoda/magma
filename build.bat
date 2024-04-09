@echo off
rem Magma - Abstraction layer over Khronos Vulkan API.
rem Copyright (C) 2018-2023 Victor Coda.

rem This program is free software: you can redistribute it and/or modify
rem it under the terms of the GNU General Public License as published by
rem the Free Software Foundation, either version 3 of the License, or
rem (at your option) any later version.

rem This program is distributed in the hope that it will be useful,
rem but WITHOUT ANY WARRANTY; without even the implied warranty of
rem MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
rem GNU General Public License for more details.

rem You should have received a copy of the GNU General Public License
rem along with this program. If not, see <https://www.gnu.org/licenses/>.

if not defined DEBUG (
  set DEBUG=1
)

set PLATFORM=VK_USE_PLATFORM_WIN32_KHR
set ARCH=x64

set PROJECT_DIR=projects\vs

if %DEBUG%==1 (
  set BUILD_DIR=%PROJECT_DIR%\%ARCH%\Debug
) else (
  set BUILD_DIR=%PROJECT_DIR%\%ARCH%\Release
)

set GLSL_DIR=src\auxiliary\spirv
set SPIRV_DIR=%GLSL_DIR%\output

set LIBRARY=magma
set PCH_FILE=%BUILD_DIR%\%LIBRARY%.pch
set PDB_FILE=%BUILD_DIR%\%LIBRARY%.pdb
set TARGET=%LIBRARY%.lib

set BASE_CFLAGS=/c /std:c++14 /EHsc /GS /Gd /permissive- /MP /nologo ^
  /W4 /WX /wd"4307" /wd"4324" /wd"4458" ^
  /constexpr:depth2048 /fp:precise /Zc:wchar_t /Zc:forScope /Zc:inline /errorReport:prompt ^
  /I%VK_SDK_PATH%\Include /Icore ^
  /D %PLATFORM% /D "_UNICODE" /D "UNICODE" ^
  /Fo%BUILD_DIR%"\\" /Fd%PDB_FILE%

if %DEBUG%==1 (
  set CFLAGS=%BASE_CFLAGS% /Od /ZI /Gm- /sdl /RTC1 /MDd /D _DEBUG
) else (
  set CFLAGS=%BASE_CFLAGS% /O2 /Oi /GL /Zi /Gm- /Gy /MT /D NDEBUG
)

set BASE_LDFLAGS=/machine:X64 /libpath:%BUILD_DIR% /out:%BUILD_DIR%\%TARGET% /nologo /ignore:4221

if %DEBUG%==1 (
  set LDFLAGS=%BASE_LDFLAGS%
) else (
  set LDFLAGS=%BASE_LDFLAGS% /LTCG
)

set glslc=%VK_SDK_PATH%\Bin\glslangValidator

set MAGMA_SRC= ^
src\allocator\alignedAllocator.cpp ^
src\allocator\allocationCallbacks.cpp ^
src\allocator\cxxAllocator.cpp ^
src\allocator\debugAlignedAllocator.cpp ^
src\allocator\deviceMemoryAllocator.cpp ^
src\allocator\vmaImplementation.cpp ^
src\auxiliary\accumulationBuffer.cpp ^
src\auxiliary\baseFramebuffer.cpp ^
src\auxiliary\blitRectangle.cpp ^
src\auxiliary\colorFramebuffer.cpp ^
src\auxiliary\colorMultisampleFramebuffer.cpp ^
src\auxiliary\depthFramebuffer.cpp ^
src\auxiliary\fillRectangleVertexShader.cpp ^
src\auxiliary\frameGrabber.cpp ^
src\auxiliary\generateMipmap.cpp ^
src\auxiliary\graphicsPipelineCache.cpp ^
src\auxiliary\imageDescriptorSet.cpp ^
src\auxiliary\immediateRender.cpp ^
src\auxiliary\multiAttachmentFramebuffer.cpp ^
src\auxiliary\profiler.cpp ^
src\auxiliary\sprite.cpp ^
src\auxiliary\swapchainFramebuffer.cpp ^
src\auxiliary\textShader.cpp ^
src\barriers\bufferMemoryBarrier.cpp ^
src\barriers\imageMemoryBarrier.cpp ^
src\descriptors\accelerationStructureDescriptor.cpp ^
src\descriptors\bufferDescriptor.cpp ^
src\descriptors\imageDescriptor.cpp ^
src\exceptions\errorResult.cpp ^
src\exceptions\exception.cpp ^
src\exceptions\reflectionErrorResult.cpp ^
src\extensions\extensions.cpp ^
src\extensions\instanceExtensions.cpp ^
src\extensions\instanceLayers.cpp ^
src\extensions\layers.cpp ^
src\extensions\physicalDeviceExtensions.cpp ^
src\helpers\executeCommandBuffer.cpp ^
src\helpers\executeDeferredOperation.cpp ^
src\helpers\streamInsertOperators.cpp ^
src\helpers\stringifyFlags.cpp ^
src\misc\application.cpp ^
src\misc\deviceFaultInfo.cpp ^
src\misc\deviceFeatures.cpp ^
src\misc\deviceQueueDescriptor.cpp ^
src\misc\format.cpp ^
src\misc\framebufferAttachmentImage.cpp ^
src\misc\geometry.cpp ^
src\misc\pipelineExecutable.cpp ^
src\misc\scopedDebugMarker.cpp ^
src\misc\sharing.cpp ^
src\misc\structureChain.cpp ^
src\objects\accelerationStructure.cpp ^
src\objects\accelerationStructureInstanceBuffer.cpp ^
src\objects\accelerationStructureStorageBuffer.cpp ^
src\objects\baseDeviceMemory.cpp ^
src\objects\buffer.cpp ^
src\objects\bufferView.cpp ^
src\objects\commandBuffer.cpp ^
src\objects\commandBufferDebug.cpp ^
src\objects\commandPool.cpp ^
src\objects\computePipeline.cpp ^
src\objects\computePipelineBatch.cpp ^
src\objects\conditionalRenderingBuffer.cpp ^
src\objects\debugReportCallback.cpp ^
src\objects\debugUtilsMessenger.cpp ^
src\objects\deferredOperation.cpp ^
src\objects\descriptorPool.cpp ^
src\objects\descriptorSet.cpp ^
src\objects\descriptorSetLayout.cpp ^
src\objects\device.cpp ^
src\objects\deviceMemory.cpp ^
src\objects\display.cpp ^
src\objects\displayMode.cpp ^
src\objects\dstTransferBuffer.cpp ^
src\objects\event.cpp ^
src\objects\externalImage.cpp ^
src\objects\fence.cpp ^
src\objects\fragmentDensityMap.cpp ^
src\objects\fragmentDensityMapView.cpp ^
src\objects\fragmentShadingRateAttachment.cpp ^
src\objects\framebuffer.cpp ^
src\objects\fullScreenExclusiveSwapchain.cpp ^
src\objects\graphicsPipeline.cpp ^
src\objects\graphicsPipelineBatch.cpp ^
src\objects\image.cpp ^
src\objects\image1D.cpp ^
src\objects\image1DArray.cpp ^
src\objects\image2D.cpp ^
src\objects\image2DArray.cpp ^
src\objects\image3D.cpp ^
src\objects\imageAttachment.cpp ^
src\objects\imageCube.cpp ^
src\objects\imagelessFramebuffer.cpp ^
src\objects\imageView.cpp ^
src\objects\indexBuffer.cpp ^
src\objects\indirectBuffer.cpp ^
src\objects\instance.cpp ^
src\objects\managedDeviceMemory.cpp ^
src\objects\mutableImage.cpp ^
src\objects\mutableImageView.cpp ^
src\objects\object.cpp ^
src\objects\physicalDevice.cpp ^
src\objects\physicalDeviceFeatures.cpp ^
src\objects\physicalDeviceGroup.cpp ^
src\objects\pipeline.cpp ^
src\objects\pipelineBatch.cpp ^
src\objects\pipelineCache.cpp ^
src\objects\pipelineLayout.cpp ^
src\objects\pipelineLibrary.cpp ^
src\objects\primaryCommandBuffer.cpp ^
src\objects\privateDataSlot.cpp ^
src\objects\queryPool.cpp ^
src\objects\queue.cpp ^
src\objects\rayTracingBuffer.cpp ^
src\objects\rayTracingPipeline.cpp ^
src\objects\rayTracingPipelineBatch.cpp ^
src\objects\renderPass.cpp ^
src\objects\renderPassMultiview.cpp ^
src\objects\resource.cpp ^
src\objects\resourcePool.cpp ^
src\objects\sampler.cpp ^
src\objects\samplerYcbcrConversion.cpp ^
src\objects\semaphore.cpp ^
src\objects\shaderModule.cpp ^
src\objects\srcTransferBuffer.cpp ^
src\objects\storageBuffer.cpp ^
src\objects\storageTexelBuffer.cpp ^
src\objects\surface.cpp ^
src\objects\swapchain.cpp ^
src\objects\swapchainImage.cpp ^
src\objects\timelineSemaphore.cpp ^
src\objects\transformFeedbackBuffer.cpp ^
src\objects\uniformTexelBuffer.cpp ^
src\objects\validationCache.cpp ^
src\objects\vertexBuffer.cpp ^
src\platform\androidHardwareBuffer.cpp ^
src\platform\androidSurface.cpp ^
src\platform\d3dExternalSemaphore.cpp ^
src\platform\externalFence.cpp ^
src\platform\externalSemaphore.cpp ^
src\platform\fuchsiaSurface.cpp ^
src\platform\iosSurface.cpp ^
src\platform\macosSurface.cpp ^
src\platform\metalSurface.cpp ^
src\platform\qnxSurface.cpp ^
src\platform\viSurface.cpp ^
src\platform\waylandSurface.cpp ^
src\platform\win32ExternalSemaphore.cpp ^
src\platform\win32Surface.cpp ^
src\platform\xcbSurface.cpp ^
src\platform\xlibSurface.cpp ^
src\renderpass\subpass.cpp ^
src\shaders\pipelineShaderStage.cpp ^
src\shaders\rayTracingShaderGroup.cpp ^
src\shaders\shaderBindingTable.cpp ^
src\shaders\shaderReflection.cpp ^
src\shaders\specialization.cpp ^
src\states\colorBlendState.cpp ^
src\states\colorBlendStateAdvanced.cpp ^
src\states\formattedOutput.cpp ^
src\states\multisampleState.cpp ^
src\states\multiViewportState.cpp ^
src\states\rasterizationState.cpp ^
src\states\vertexInputState.cpp ^
src\states\vertexInputStructure.cpp ^
src\states\viewportState.cpp

echo Compiling shaders
%glslc% -V --vn fsBlit %GLSL_DIR%\blit.frag -o %SPIRV_DIR%\blitf
%glslc% -V --vn vsBlit %GLSL_DIR%\blit.vert -o %SPIRV_DIR%\blitv
%glslc% -V -DNV --vn vsBlitNV %GLSL_DIR%\blit.vert -o %SPIRV_DIR%\blitv_nv
%glslc% -V --vn fsFont %GLSL_DIR%\font.frag -o %SPIRV_DIR%\fontf
%glslc% -V --vn vsImm %GLSL_DIR%\imm.vert -o %SPIRV_DIR%\immv
%glslc% -V --vn fsImm %GLSL_DIR%\imm.frag -o %SPIRV_DIR%\immf

if not exist %PROJECT_DIR%\%ARCH% mkdir %PROJECT_DIR%\%ARCH%
if not exist %BUILD_DIR% mkdir %BUILD_DIR%

echo Creating precompiled header
cl %CFLAGS% /Yc"pch.h" /Fp%PCH_FILE% src\core\pch.cpp

echo Compiling library
cl %CFLAGS% /Yu"pch.h" /Fp%PCH_FILE% %MAGMA_SRC%

rem Third party source files
cl %CFLAGS% src\third-party\SPIRV-Reflect\spirv_reflect.c

set MAGMA_OBJS= ^
alignedAllocator.obj ^
allocationCallbacks.obj ^
cxxAllocator.obj ^
debugAlignedAllocator.obj ^
deviceMemoryAllocator.obj ^
vmaImplementation.obj ^
accumulationBuffer.obj ^
baseFramebuffer.obj ^
blitRectangle.obj ^
colorFramebuffer.obj ^
colorMultisampleFramebuffer.obj ^
depthFramebuffer.obj ^
fillRectangleVertexShader.obj ^
frameGrabber.obj ^
generateMipmap.obj ^
graphicsPipelineCache.obj ^
imageDescriptorSet.obj ^
immediateRender.obj ^
multiAttachmentFramebuffer.obj ^
profiler.obj ^
sprite.obj ^
swapchainFramebuffer.obj ^
textShader.obj ^
bufferMemoryBarrier.obj ^
imageMemoryBarrier.obj ^
pch.obj ^
accelerationStructureDescriptor.obj ^
bufferDescriptor.obj ^
imageDescriptor.obj ^
errorResult.obj ^
exception.obj ^
reflectionErrorResult.obj ^
extensions.obj ^
instanceExtensions.obj ^
instanceLayers.obj ^
layers.obj ^
physicalDeviceExtensions.obj ^
executeCommandBuffer.obj ^
executeDeferredOperation.obj ^
streamInsertOperators.obj ^
stringifyFlags.obj ^
application.obj ^
deviceFaultInfo.obj ^
deviceFeatures.obj ^
deviceQueueDescriptor.obj ^
format.obj ^
framebufferAttachmentImage.obj ^
geometry.obj ^
pipelineExecutable.obj ^
scopedDebugMarker.obj ^
sharing.obj ^
structureChain.obj ^
accelerationStructure.obj ^
accelerationStructureInstanceBuffer.obj ^
accelerationStructureStorageBuffer.obj ^
baseDeviceMemory.obj ^
buffer.obj ^
bufferView.obj ^
commandBuffer.obj ^
commandBufferDebug.obj ^
commandPool.obj ^
computePipeline.obj ^
computePipelineBatch.obj ^
conditionalRenderingBuffer.obj ^
debugReportCallback.obj ^
debugUtilsMessenger.obj ^
deferredOperation.obj ^
descriptorPool.obj ^
descriptorSet.obj ^
descriptorSetLayout.obj ^
device.obj ^
deviceMemory.obj ^
display.obj ^
displayMode.obj ^
dstTransferBuffer.obj ^
event.obj ^
externalImage.obj ^
fence.obj ^
fragmentDensityMap.obj ^
fragmentDensityMapView.obj ^
fragmentShadingRateAttachment.obj ^
framebuffer.obj ^
fullScreenExclusiveSwapchain.obj ^
graphicsPipeline.obj ^
graphicsPipelineBatch.obj ^
image.obj ^
image1D.obj ^
image1DArray.obj ^
image2D.obj ^
image2DArray.obj ^
image3D.obj ^
imageAttachment.obj ^
imageCube.obj ^
imagelessFramebuffer.obj ^
imageView.obj ^
indexBuffer.obj ^
indirectBuffer.obj ^
instance.obj ^
managedDeviceMemory.obj ^
mutableImage.obj ^
mutableImageView.obj ^
object.obj ^
physicalDevice.obj ^
physicalDeviceFeatures.obj ^
physicalDeviceGroup.obj ^
pipeline.obj ^
pipelineBatch.obj ^
pipelineCache.obj ^
pipelineLayout.obj ^
pipelineLibrary.obj ^
primaryCommandBuffer.obj ^
privateDataSlot.obj ^
queryPool.obj ^
queue.obj ^
rayTracingBuffer.obj ^
rayTracingPipeline.obj ^
rayTracingPipelineBatch.obj ^
renderPass.obj ^
renderPassMultiview.obj ^
resource.obj ^
resourcePool.obj ^
sampler.obj ^
samplerYcbcrConversion.obj ^
semaphore.obj ^
shaderModule.obj ^
srcTransferBuffer.obj ^
storageBuffer.obj ^
storageTexelBuffer.obj ^
surface.obj ^
swapchain.obj ^
swapchainImage.obj ^
timelineSemaphore.obj ^
transformFeedbackBuffer.obj ^
uniformTexelBuffer.obj ^
validationCache.obj ^
vertexBuffer.obj ^
androidHardwareBuffer.obj ^
androidSurface.obj ^
d3dExternalSemaphore.obj ^
externalFence.obj ^
externalSemaphore.obj ^
fuchsiaSurface.obj ^
iosSurface.obj ^
macosSurface.obj ^
metalSurface.obj ^
qnxSurface.obj ^
viSurface.obj ^
waylandSurface.obj ^
win32ExternalSemaphore.obj ^
win32Surface.obj ^
xcbSurface.obj ^
xlibSurface.obj ^
subpass.obj ^
pipelineShaderStage.obj ^
rayTracingShaderGroup.obj ^
shaderBindingTable.obj ^
shaderReflection.obj ^
specialization.obj ^
colorBlendState.obj ^
colorBlendStateAdvanced.obj ^
formattedOutput.obj ^
multisampleState.obj ^
multiViewportState.obj ^
rasterizationState.obj ^
vertexInputState.obj ^
vertexInputStructure.obj ^
viewportState.obj ^
spirv_reflect.obj

echo Linking library %BUILD_DIR%\%TARGET%
lib %LDFLAGS% %MAGMA_OBJS%
