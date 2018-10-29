#version 450

layout(location = 0) out vec4 oColor;
layout(binding = 0) uniform sampler2D s0;

void main()
{
    oColor = texture(s0, gl_FragCoord.xy);
}
