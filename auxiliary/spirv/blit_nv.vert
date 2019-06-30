#version 450

layout(location = 0) out vec2 oUV;

out gl_PerVertex {
  vec4 gl_Position;
};

void main()
{
  // https://www.khronos.org/registry/OpenGL/extensions/NV/NV_fill_rectangle.txt
  vec2 fill_rect[3] = vec2[3](
    vec2(-1.,-1.),
    vec2(-1., 1.),
    vec2( 1.,-1.));
  vec2 pos = fill_rect[gl_VertexIndex];
  oUV = pos * 0.5 + 0.5;
  gl_Position = vec4(pos, 0., 1.);
}
