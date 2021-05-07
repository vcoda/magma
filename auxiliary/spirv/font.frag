#version 450

#define GLYPH_SIZE vec2(8, 12)

struct String
{
  vec2 pos;
  uint first, last;
  vec4 color;
};

layout(set = 0, binding = 0) uniform perFrame {
  uint stringCount;
};

layout(set = 0, binding = 1) readonly buffer stringBuffer {
    String strings[];
};

layout(set = 0, binding = 2) readonly buffer glyphBuffer {
    vec4 glyphs[];
};

layout(location = 0) out vec4 oColor;

float extractBit(float n, float b)
{
  return floor(mod(floor(n / pow(2., floor(b))), 2.));
}

float sprite(vec4 glyph, vec2 size, vec2 pos)
{
  pos = floor(pos);
  float bit = (size.x - pos.x - 1.) + (size.y - pos.y) * size.x;
  float alpha = extractBit(glyph.x, bit - 72.) +
    extractBit(glyph.y, bit - 48.) +
    extractBit(glyph.z, bit - 24.) +
    extractBit(glyph.w, bit);
  bvec4 bounds;
  bounds.xy = greaterThanEqual(pos, vec2(0.));
  bounds.zw = lessThan(pos, size);
  return all(bounds) ? alpha : 0.;
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
      float alpha = sprite(glyphs[j], GLYPH_SIZE, fragCoord - s.pos);
      oColor += s.color * alpha;
    }
  }
  if (0. == oColor.a)
      discard;
}
