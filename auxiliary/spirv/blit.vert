#version 450

layout(location = 0) out vec2 oUV;

out gl_PerVertex {
  vec4 gl_Position;
};

void main()
{
  // Screen aligned triangle without being split
  // into two triangles with an internal edge.
  vec2 tri[3] = vec2[3](
    vec2(-1.,-1.),
    vec2(-1., 3.),
    vec2( 3.,-1.));
  vec2 pos = tri[gl_VertexIndex];
  oUV = pos * 0.5 + 0.5;
  gl_Position = vec4(pos, 0., 1.);
}
