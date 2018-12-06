#version 450

layout(location = 0) out vec2 oTexCoord;

out gl_PerVertex
{
  vec4 gl_Position;
};

void main()
{
  vec2 positions[4] = vec2[4](
    vec2(-1.,-1.),
    vec2(-1., 1.),
    vec2( 1.,-1.),
    vec2( 1., 1.));
  gl_Position = vec4(positions[gl_VertexIndex], 0., 1.);
  oTexCoord = gl_Position.xy * 0.5 + 0.5;
}
