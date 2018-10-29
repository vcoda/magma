#version 450

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normalPointSize;
layout(location = 2) in vec4 color;
layout(location = 3) in vec2 texCoord;
layout(location = 0) out vec3 oNormal;
layout(location = 1) out vec4 oColor;
layout(location = 2) out vec2 oTexCoord;

layout(push_constant) uniform PushConstants {
    mat4 transform;
};

out gl_PerVertex {
    vec4 gl_Position;
    float gl_PointSize;
};

void main()
{
    oNormal = normalPointSize.xyz;
    oColor = color;
    oTexCoord = texCoord;
    gl_Position = transform * position;
    gl_PointSize = normalPointSize.w;
}
