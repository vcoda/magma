// "96-bit 8x12 Font" by Flyguy: https://www.shadertoy.com/view/Mt2GWD
#version 450

#define GLYPH_SIZE vec2(8, 12)

struct String
{
  vec2 pos;
  uint first, last;
  vec4 color;
};

layout(push_constant) uniform pushConstants {
  uint stringCount;
};

layout(set = 0, binding = 0) readonly buffer stringBuffer {
  String strings[];
};

layout(set = 0, binding = 1) readonly buffer charBuffer {
  uvec4 chars[];
};

layout(location = 0) out vec4 oColor;

bool extractBit(uint n, uint b)
{
  return (n & (1 << b)) > 0 && (b < 24);
}

bool sprite(uvec4 glyph, vec2 size, vec2 pos)
{
  bvec4 bounds;
  pos = floor(pos);
  bounds.xy = greaterThanEqual(pos, 0.0.xx);
  bounds.zw = lessThanEqual(pos, size);
  if (all(bounds)) // clip
  {
    vec2 d = size - pos;
    uvec4 bits = uvec4(d.x + d.y * size.x) - uvec4(72, 48, 24, 0);
    return
      extractBit(glyph.x, bits.x) ||
      extractBit(glyph.y, bits.y) ||
      extractBit(glyph.z, bits.z) ||
      extractBit(glyph.w, bits.w);
  }
  return false;
}

void main()
{
  vec2 fragCoord = floor(gl_FragCoord.xy);
  oColor = 0.0.xxxx;
  for (uint i = 0; i < stringCount; ++i)
  {
    String s = strings[i];
    for (uint j = s.first; j <= s.last; ++j)
    {
      bool mask = sprite(chars[j], GLYPH_SIZE, fragCoord - s.pos);
      oColor += s.color * (mask ? 1. : 0.);
      s.pos.x += GLYPH_SIZE.x;
    }
  }
  if (0. == oColor.a)
      discard;
}
