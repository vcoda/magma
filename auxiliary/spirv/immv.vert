#version 450

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normalPointSize;
layout(location = 2) in vec4 color;
layout(location = 3) in vec2 uv;
layout(location = 0) out vec3 oNormal;
layout(location = 1) out vec4 oColor;
layout(location = 2) out vec2 oUV;

out gl_PerVertex {
  float gl_PointSize;
  vec4 gl_Position;
};

layout(push_constant) uniform PushConstants {
  mat4 worldViewProj;
};

void main()
{
  oNormal = normalPointSize.xyz;
  oColor = color;
  oUV = uv;
  gl_PointSize = normalPointSize.w;
  gl_Position = worldViewProj * position;
}
