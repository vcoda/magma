#version 450

layout(location = 0) out vec2 oTexCoord;

out gl_PerVertex {
  vec4 gl_Position;
};

void main()
{
  vec2 tri[3] = vec2[3](
#ifdef NV // https://www.khronos.org/registry/OpenGL/extensions/NV/NV_fill_rectangle.txt
    vec2(-1., -1.), vec2(-1., 1.), vec2(1., -1.));
#else
    vec2(-1., -1.), vec2(-1., 3.), vec2(3., -1.));
#endif
  vec2 pos = tri[gl_VertexIndex];
  oTexCoord = pos * .5 + .5;
  gl_Position = vec4(pos, 0., 1.);
}
