#version 450

layout(location = 0) in vec3 normal;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 texCoord;
layout(location = 0) out vec4 oColor;

void main()
{
  oColor = color;
}
