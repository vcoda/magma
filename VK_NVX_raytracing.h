#define VK_NVX_RAYTRACING_EXTENSION_NAME "VK_NVX_raytracing"

VK_DEFINE_NON_DISPATCHABLE_HANDLE(VkAccelerationStructureNVX)

typedef struct VkRaytracingPipelineCreateInfoNVX {
    VkStructureType                           sType;
    const void*                               pNext;
    VkPipelineCreateFlags                     flags;
    uint32_t                                  stageCount;
    const VkPipelineShaderStageCreateInfo*    pStages;
    const uint32_t*                           pGroupNumbers;
    uint32_t                                  maxRecursionDepth;
    VkPipelineLayout                          layout;
    VkPipeline                                basePipelineHandle;
    int32_t                                   basePipelineIndex;
} VkRaytracingPipelineCreateInfoNVX;

typedef enum VkAccelerationStructureTypeNVX {
    VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_NVX = 0x0,
    VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_NVX = 0x1
} VkAccelerationStructureTypeNVX;

typedef enum VkBuildAccelerationStructureFlagBitsNVX {
    VK_BUILD_ACCELERATION_STRUCTURE_ALLOW_UPDATE_BIT_NVX = 0x00000001,
    VK_BUILD_ACCELERATION_STRUCTURE_ALLOW_COMPACTION_BIT_NVX = 0x00000002,
    VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_NVX = 0x00000004,
    VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_BUILD_BIT_NVX = 0x00000008,
    VK_BUILD_ACCELERATION_STRUCTURE_LOW_MEMORY_BIT_NVX = 0x00000010,
} VkBuildAccelerationStructureFlagBitsNVX;
typedef VkFlags VkBuildAccelerationStructureFlagsNVX;

typedef enum VkGeometryTypeNVX {
    VK_GEOMETRY_TYPE_TRIANGLES_NVX = 0,
    VK_GEOMETRY_TYPE_AABBS_NVX = 1,
} VkGeometryTypeNVX;

typedef enum VkCopyAccelerationStructureModeNVX {
    VK_COPY_ACCELERATION_STRUCTURE_MODE_CLONE_NVX = 0,
    VK_COPY_ACCELERATION_STRUCTURE_MODE_COMPACT_NVX = 1,
} VkCopyAccelerationStructureModeNVX;

typedef struct VkGeometryTrianglesNVX {
    VkStructureType    sType;
    const void*        pNext;
    VkBuffer           vertexData;
    VkDeviceSize       vertexOffset;
    uint32_t           vertexCount;
    VkDeviceSize       vertexStride;
    VkFormat           vertexFormat;
    VkBuffer           indexData;
    VkDeviceSize       indexOffset;
    uint32_t           indexCount;
    VkIndexType        indexType;
    VkBuffer           transformData;
    VkDeviceSize       transformOffset;
} VkGeometryTrianglesNVX;

typedef struct VkGeometryAABBNVX {
    VkStructureType    sType;
    const void*        pNext;
    VkBuffer           aabbData;
    uint32_t           numAABBs;
    uint32_t           stride;
    VkDeviceSize       offset;
} VkGeometryAABBNVX;

typedef struct VkGeometryDataNVX {
    VkGeometryTrianglesNVX    triangles;
    VkGeometryAABBNVX         aabbs;
} VkGeometryDataNVX;

typedef enum VkGeometryFlagBitsNVX {
    VK_GEOMETRY_OPAQUE_BIT_NVX = 0x00000001,
    VK_GEOMETRY_NO_DUPLICATE_ANY_HIT_INVOCATION_BIT_NVX = 0x00000002,
} VkGeometryFlagBitsNVX;
typedef VkFlags VkGeometryFlagsNVX;

typedef struct VkGeometryNVX {
    VkStructureType       sType;
    const void*           pNext;
    VkGeometryTypeNVX     geometryType;
    VkGeometryDataNVX     geometry;
    VkGeometryFlagsNVX    flags;
} VkGeometryNVX;

typedef struct VkAccelerationStructureCreateInfoNVX {
    VkStructureType                         sType;
    const void*                             pNext;
    VkAccelerationStructureTypeNVX          type;
    VkBuildAccelerationStructureFlagsNVX    flags;
    VkDeviceSize                            compactedSize;
    uint32_t                                instanceCount;
    uint32_t                                geometryCount;
    const VkGeometryNVX*                    pGeometries;
} VkAccelerationStructureCreateInfoNVX;

typedef struct VkAccelerationStructureMemoryRequirementsInfoNVX {
    VkStructureType               sType;
    const void*                   pNext;
    VkAccelerationStructureNVX    accelerationStructure;
} VkAccelerationStructureMemoryRequirementsInfoNVX;

typedef struct VkBindAccelerationStructureMemoryInfoNVX {
    VkStructureType               sType;
    const void*                   pNext;
    VkAccelerationStructureNVX    accelerationStructure;
    VkDeviceMemory                memory;
    VkDeviceSize                  memoryOffset;
    uint32_t                      deviceIndexCount;
    const uint32_t*               pDeviceIndices;
} VkBindAccelerationStructureMemoryInfoNVX;


//////////////////////////

VkResult vkCreateRaytracingPipelinesNVX(
    VkDevice                                    device,
    VkPipelineCache                             pipelineCache,
    uint32_t                                    createInfoCount,
    const VkRaytracingPipelineCreateInfoNVX*    pCreateInfos,
    const VkAllocationCallbacks*                pAllocator,
    VkPipeline*                                 pPipelines);

VkResult vkCompileDeferredNVX(
    VkDevice                                    device,
    VkPipeline                                  pipeline,
    uint32_t                                    shader);

VkResult vkGetRaytracingShaderHandlesNVX(
    VkDevice                                    device,
    VkPipeline                                  pipeline,
    uint32_t                                    firstGroup,
    uint32_t                                    groupCount,
    size_t                                      dataSize,
    void*                                       pData);

VkResult vkCreateAccelerationStructureNVX(
    VkDevice                                    device,
    const VkAccelerationStructureCreateInfoNVX* pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkAccelerationStructureNVX*                 pAccelerationStructure);

void vkDestroyAccelerationStructureNVX(
    VkDevice                                    device,
    VkAccelerationStructureNVX                  accelerationStructure,
    const VkAllocationCallbacks*                pAllocator);

void vkGetAccelerationStructureMemoryRequirementsNVX(
    VkDevice                                    device,
    const VkAccelerationStructureMemoryRequirementsInfoNVX* pInfo,
    VkMemoryRequirements2KHR*                   pMemoryRequirements);

void vkGetAccelerationStructureScratchMemoryRequirementsNVX(
    VkDevice                                    device,
    const VkAccelerationStructureMemoryRequirementsInfoNVX* pInfo,
    VkMemoryRequirements2KHR*                   pMemoryRequirements);

VkResult vkBindAccelerationStructureMemoryNVX(
    VkDevice                                    device,
    uint32_t                                    bindInfoCount,
    const VkBindAccelerationStructureMemoryInfoNVX* pBindInfos);

VkResult vkGetAccelerationStructureHandleNVX(
    VkDevice                                    device,
    VkAccelerationStructureNVX                  accelerationStructure,
    size_t                                      dataSize,
    void*                                       pData);

void vkCmdBuildAccelerationStructureNVX(
    VkCommandBuffer                             cmdBuf,
    VkAccelerationStructureTypeNVX              type,
    uint32_t                                    instanceCount,
    VkBuffer                                    instanceData,
    VkDeviceSize                                instanceOffset,
    uint32_t                                    geometryCount,
    const VkGeometryNVX*                        pGeometries,
    VkBuildAccelerationStructureFlagsNVX        flags,
    VkBool32                                    update,
    VkAccelerationStructureNVX                  dst,
    VkAccelerationStructureNVX                  src,
    VkBuffer                                    scratch,
    VkDeviceSize                                scratchOffset);

void vkCmdWriteAccelerationStructurePropertiesNVX(
    VkCommandBuffer                             cmdBuf,
    VkAccelerationStructureNVX                  accelerationStructure,
    VkQueryType                                 queryType,
    VkQueryPool                                 queryPool,
    uint32_t                                    query);

void vkCmdCopyAccelerationStructureNVX(
    VkCommandBuffer                             cmdBuf,
    VkAccelerationStructureNVX                  dst,
    VkAccelerationStructureNVX                  src,
    VkCopyAccelerationStructureModeNVX          mode);

void vkCmdTraceRaysNVX(
    VkCommandBuffer                             cmdBuf,
    VkBuffer                                    raygenShaderBindingTableBuffer,
    VkDeviceSize                                raygenShaderBindingOffset,
    VkBuffer                                    missShaderBindingTableBuffer,
    VkDeviceSize                                missShaderBindingOffset,
    VkDeviceSize                                missShaderBindingStride,
    VkBuffer                                    hitShaderBindingTableBuffer,
    VkDeviceSize                                hitShaderBindingOffset,
    VkDeviceSize                                hitShaderBindingStride,
    uint32_t                                    width,
    uint32_t                                    height);

/////////////////////////

typedef VkResult (VKAPI_PTR *PFN_vkCreateRaytracingPipelinesNVX)(
    VkDevice                                    device,
    VkPipelineCache                             pipelineCache,
    uint32_t                                    createInfoCount,
    const VkRaytracingPipelineCreateInfoNVX*    pCreateInfos,
    const VkAllocationCallbacks*                pAllocator,
    VkPipeline*                                 pPipelines);

typedef VkResult (VKAPI_PTR *PFN_vkCompileDeferredNVX)(
    VkDevice                                    device,
    VkPipeline                                  pipeline,
    uint32_t                                    shader);

typedef VkResult (VKAPI_PTR *PFN_vkGetRaytracingShaderHandlesNVX)(
    VkDevice                                    device,
    VkPipeline                                  pipeline,
    uint32_t                                    firstGroup,
    uint32_t                                    groupCount,
    size_t                                      dataSize,
    void*                                       pData);

typedef VkResult (VKAPI_PTR *PFN_vkCreateAccelerationStructureNVX)(
    VkDevice                                    device,
    const VkAccelerationStructureCreateInfoNVX* pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkAccelerationStructureNVX*                 pAccelerationStructure);

typedef void (VKAPI_PTR *PFN_vkDestroyAccelerationStructureNVX)(
    VkDevice                                    device,
    VkAccelerationStructureNVX                  accelerationStructure,
    const VkAllocationCallbacks*                pAllocator);

typedef void (VKAPI_PTR *PFN_vkGetAccelerationStructureMemoryRequirementsNVX)(
    VkDevice                                    device,
    const VkAccelerationStructureMemoryRequirementsInfoNVX* pInfo,
    VkMemoryRequirements2KHR*                   pMemoryRequirements);

typedef void (VKAPI_PTR *PFN_vkGetAccelerationStructureScratchMemoryRequirementsNVX)(
    VkDevice                                    device,
    const VkAccelerationStructureMemoryRequirementsInfoNVX* pInfo,
    VkMemoryRequirements2KHR*                   pMemoryRequirements);

typedef VkResult (VKAPI_PTR *PFN_vkBindAccelerationStructureMemoryNVX)(
    VkDevice                                    device,
    uint32_t                                    bindInfoCount,
    const VkBindAccelerationStructureMemoryInfoNVX* pBindInfos);

typedef VkResult (VKAPI_PTR *PFN_vkGetAccelerationStructureHandleNVX)(
    VkDevice                                    device,
    VkAccelerationStructureNVX                  accelerationStructure,
    size_t                                      dataSize,
    void*                                       pData);

typedef void (VKAPI_PTR *PFN_vkCmdBuildAccelerationStructureNVX)(
    VkCommandBuffer                             cmdBuf,
    VkAccelerationStructureTypeNVX              type,
    uint32_t                                    instanceCount,
    VkBuffer                                    instanceData,
    VkDeviceSize                                instanceOffset,
    uint32_t                                    geometryCount,
    const VkGeometryNVX*                        pGeometries,
    VkBuildAccelerationStructureFlagsNVX        flags,
    VkBool32                                    update,
    VkAccelerationStructureNVX                  dst,
    VkAccelerationStructureNVX                  src,
    VkBuffer                                    scratch,
    VkDeviceSize                                scratchOffset);

typedef void (VKAPI_PTR *PFN_vkCmdWriteAccelerationStructurePropertiesNVX)(
    VkCommandBuffer                             cmdBuf,
    VkAccelerationStructureNVX                  accelerationStructure,
    VkQueryType                                 queryType,
    VkQueryPool                                 queryPool,
    uint32_t                                    query);

typedef void (VKAPI_PTR *PFN_vkCmdCopyAccelerationStructureNVX)(
    VkCommandBuffer                             cmdBuf,
    VkAccelerationStructureNVX                  dst,
    VkAccelerationStructureNVX                  src,
    VkCopyAccelerationStructureModeNVX          mode);

typedef void (VKAPI_PTR *PFN_vkCmdTraceRaysNVX)(
    VkCommandBuffer                             cmdBuf,
    VkBuffer                                    raygenShaderBindingTableBuffer,
    VkDeviceSize                                raygenShaderBindingOffset,
    VkBuffer                                    missShaderBindingTableBuffer,
    VkDeviceSize                                missShaderBindingOffset,
    VkDeviceSize                                missShaderBindingStride,
    VkBuffer                                    hitShaderBindingTableBuffer,
    VkDeviceSize                                hitShaderBindingOffset,
    VkDeviceSize                                hitShaderBindingStride,
    uint32_t                                    width,
    uint32_t                                    height);

/////////////////////////

#define VK_STRUCTURE_TYPE_RAYTRACING_PIPELINE_CREATE_INFO_NVX 1000165000
#define VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_NVX 1000165001

#define VK_STRUCTURE_TYPE_GEOMETRY_INSTANCE_NVX 1000165002
#define VK_STRUCTURE_TYPE_GEOMETRY_NVX 1000165003
#define VK_STRUCTURE_TYPE_GEOMETRY_TRIANGLES_NVX 1000165004
#define VK_STRUCTURE_TYPE_GEOMETRY_AABB_NVX 1000165005
#define VK_STRUCTURE_TYPE_BIND_ACCELERATION_STRUCTURE_MEMORY_INFO_NVX 1000165006
#define VK_STRUCTURE_TYPE_DESCRIPTOR_ACCELERATION_STRUCTURE_INFO_NVX 1000165007
#define VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_MEMORY_REQUIREMENTS_INFO_NVX 1000165008

#define VK_DEBUG_REPORT_OBJECT_TYPE_ACCELERATION_STRUCTURE_NVX_EXT 1000165000

#define VK_PIPELINE_BIND_POINT_RAYTRACING_NVX 1000165000

#define VK_QUERY_TYPE_COMPACTED_SIZE_NVX 1000165000

#define VK_SHADER_STAGE_RAYGEN_BIT_NVX 0x00000100
#define VK_SHADER_STAGE_ANY_HIT_BIT_NVX 0x00000200
#define VK_SHADER_STAGE_CLOSEST_HIT_BIT_NVX 0x00000400
#define VK_SHADER_STAGE_MISS_BIT_NVX 0x00000800
#define VK_SHADER_STAGE_INTERSECTION_BIT_NVX 0x00001000
#define VK_SHADER_STAGE_CALLABLE_BIT_NVX 0x00002000
#define VK_SHADER_STAGE_TASK_BIT_NV 0x00000040
#define VK_SHADER_STAGE_MESH_BIT_NV 0x00000080

#define VK_BUFFER_USAGE_RAYTRACING_BIT_NVX 0x00000400
