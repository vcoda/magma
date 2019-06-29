#version 450

layout(location = 0) out vec2 oUV;

out gl_PerVertex {
  vec4 gl_Position;
};

void main()
{
  vec2 rc[4] = vec2[4](
    vec2(-1.,-1.),
    vec2(-1., 1.),
    vec2( 1.,-1.),
    vec2( 1., 1.));
  vec2 pos = rc[gl_VertexIndex];
  oUV = pos * 0.5 + 0.5;
  gl_Position = vec4(pos, 0., 1.);
}
