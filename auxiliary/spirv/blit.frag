#version 450

layout(binding = 0) uniform sampler2D s0;

layout(location = 0) in vec2 texCoord;
layout(location = 0) out vec4 oColor;

void main()
{
  oColor = texture(s0, texCoord);
}
