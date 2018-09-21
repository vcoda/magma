#define VK_NVX_RAYTRACING_INFO_EXTENSION_NAME "VK_NVX_raytracing"

VK_DEFINE_NON_DISPATCHABLE_HANDLE(VkAccelerationStructureNVX);

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
    VK_ACCELERATION_STRUCTURE_TYPE_UNKNOWN = 0x0
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

VkResult vkCreateAccelerationStructureNVX(
    VkDevice                                    device,
    const VkAccelerationStructureCreateInfoNVX* pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkAccelerationStructureNVX*                 pAccelerationStructure);

void vkDestroyAccelerationStructureNVX(
    VkDevice                                    device,
    VkAccelerationStructureNVX                  accelerationStructure,
    const VkAllocationCallbacks*                pAllocator);

typedef enum VkCopyAccelerationStructureModeNVX {
    VK_COPY_ACCELERATION_STRUCTURE_MODE_CLONE_NVX = 0,
    VK_COPY_ACCELERATION_STRUCTURE_MODE_COMPACT_NVX = 1,
} VkCopyAccelerationStructureModeNVX;

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

typedef VkResult (VKAPI_PTR *PFN_vkCreateAccelerationStructureNVX)(
    VkDevice                                    device,
    const VkAccelerationStructureCreateInfoNVX* pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkAccelerationStructureNVX*                 pAccelerationStructure);

typedef void (VKAPI_PTR *PFN_vkDestroyAccelerationStructureNVX)(
    VkDevice                                    device,
    VkAccelerationStructureNVX                  accelerationStructure,
    const VkAllocationCallbacks*                pAllocator);

/////////////////////////

#define VK_STRUCTURE_TYPE_RAYTRACING_PIPELINE_CREATE_INFO_NVX 1000165000
#define VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_NVX 1000165001

#define VK_DEBUG_REPORT_OBJECT_TYPE_ACCELERATION_STRUCTURE_NVX_EXT 1000165000