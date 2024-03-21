// 96-bit 8x12 Font
// Created by Flyguy in 2015-04-16
// https://www.shadertoy.com/view/Mt2GWD
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

bool extractBit(uint n, uint k)
{
  return (n & (1 << k)) > 0 && (k < 24);
}

bool sprite(uvec4 glyph, vec2 size, vec2 pos)
{
  pos = floor(pos);
  bvec4 bounds;
  bounds.xy = greaterThanEqual(pos, vec2(0.));
  bounds.zw = lessThanEqual(pos, size);
  if (all(bounds))
  {
    vec2 d = size - pos;
    uint bit = uint(d.x + d.y * size.x);
    return
      extractBit(glyph.x, bit - 72) ||
      extractBit(glyph.y, bit - 48) ||
      extractBit(glyph.z, bit - 24) ||
      extractBit(glyph.w, bit);
  }
  return false;
}

void main()
{
  vec2 fragCoord = floor(gl_FragCoord.xy);
  oColor = vec4(0.);
  for (uint i = 0; i < stringCount; ++i)
  {
    String s = strings[i];
    for (uint j = s.first; j <= s.last; ++j, s.pos.x += GLYPH_SIZE.x)
    {
      bool mask = sprite(chars[j], GLYPH_SIZE, fragCoord - s.pos);
      oColor += s.color * (mask ? 1. : 0.);
    }
  }
  if (0. == oColor.a)
      discard;
}
