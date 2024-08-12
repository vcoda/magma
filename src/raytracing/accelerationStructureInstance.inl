namespace magma
{
#ifdef VK_KHR_acceleration_structure
constexpr TransformMatrix::TransformMatrix() noexcept:
    VkTransformMatrixKHR{
        {   // identity
            {1, 0, 0, 0},
            {0, 1, 0, 0},
            {0, 0, 1, 0}
        }
    }
{}

template<int M, int N>
constexpr TransformMatrix::TransformMatrix(const float (&mat)[M][N]) noexcept
{
    static_assert(M <= 4, "the number of rows in the matrix is greater than 4");
    static_assert(N <= 4, "the number of columns in the matrix is greater than 4");
    for (int i = 0; i < std::min(M, 3); ++i)
        for (int j = 0; j < N; ++j)
            matrix[i][j] = mat[i][j];
    if constexpr (N < 4)
    {   // Zero translation in case of 3x3 matrix
        matrix[0][3] = 0.f;
        matrix[1][3] = 0.f;
        matrix[2][3] = 0.f;
    }
}

template<class Matrix>
inline TransformMatrix::TransformMatrix(const Matrix& mat) noexcept
{
    float affine[3][4];
    mat.store(affine); // Type must have store() method
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 4; ++j)
            matrix[i][j] = affine[i][j];
}

constexpr AccelerationStructureInstance::AccelerationStructureInstance() noexcept:
    VkAccelerationStructureInstanceKHR{
        TransformMatrix(), // transform
        0, // instanceCustomIndex
        0xFF, // mask
        0, // instanceShaderBindingTableRecordOffset
        0, // flags
        0ull // accelerationStructureReference
    }
{}
#endif // VK_KHR_acceleration_structure

#ifdef VK_NV_ray_tracing_motion_blur
constexpr SRTData::SRTData() noexcept:
    VkSRTDataNV{
        1, // sx is the x component of the scale of the transform
        0, // a is one component of the shear for the transform
        0, // b is one component of the shear for the transform
        0, // pvx is the x component of the pivot point of the transform
        1, // sy is the y component of the scale of the transform
        0, // c is one component of the shear for the transform
        0, // pvy is the y component of the pivot point of the transform
        1, // sz is the z component of the scale of the transform
        0, // pvz is the z component of the pivot point of the transform
        0, // qx is the x component of the rotation quaternion
        0, // qy is the y component of the rotation quaternion
        0, // qz is the z component of the rotation quaternion
        1, // qw is the w component of the rotation quaternion
        0, // tx is the x component of the post-rotation translation
        0, // ty is the y component of the post-rotation translation
        0  // tz is the z component of the post-rotation translation
    }
{}

constexpr AccelerationStructureMatrixMotionInstance::AccelerationStructureMatrixMotionInstance() noexcept:
    VkAccelerationStructureMatrixMotionInstanceNV{
        TransformMatrix(), // transformT0
        TransformMatrix(), // transformT1
        0, // instanceCustomIndex
        0xFF, // mask
        0, // instanceShaderBindingTableRecordOffset
        0, // flags
        0ull // accelerationStructureReference
    }
{}

constexpr AccelerationStructureSRTMotionInstance::AccelerationStructureSRTMotionInstance() noexcept:
    VkAccelerationStructureSRTMotionInstanceNV{
        SRTData(), // transformT0
        SRTData(), // transformT1
        0, // instanceCustomIndex
        0xFF, // mask
        0, // instanceShaderBindingTableRecordOffset
        0, // flags
        0ull // accelerationStructureReference
    }
{}

constexpr AccelerationStructureMotionInstance::AccelerationStructureMotionInstance() noexcept:
    VkAccelerationStructureMotionInstanceNV{
        VK_ACCELERATION_STRUCTURE_MOTION_INSTANCE_TYPE_STATIC_NV,
        0,
        AccelerationStructureInstance()
    }
{
    static_assert(sizeof(VkAccelerationStructureMotionInstanceNV) == 152,
        "VkAccelerationStructureMotionInstanceNV structure size mismatch");
    static_assert(sizeof(AccelerationStructureMotionInstance) == 160,
        "VkAccelerationStructureMotionInstanceNV must have a stride of 160 bytes");
}
#endif // VK_NV_ray_tracing_motion_blur
} // namespace magma
