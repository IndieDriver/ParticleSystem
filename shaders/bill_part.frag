#version 330
uniform vec3 cursorPos;

in GeomData {
  vec2 uv;
  vec3 pos;
} gs_data;
out vec4 frag_colour;

uniform float iTime;
uniform sampler2D sampler_tex;

vec3 palette(in float t, in vec3 a, in vec3 b, in vec3 c, in vec3 d){
    return a + b * cos(6.28318 * (c * t + d));
}

void main () {
  float dist = distance(cursorPos, gs_data.pos);
  vec3 color = palette(dist * 0.1, vec3(0.5, 0.5, 0.5), vec3(0.5, 0.5, 0.5), vec3(1.0, 1.0, 1.0), vec3(0.0, 0.10, 0.20));
  vec4 tex_color = texture(sampler_tex, gs_data.uv);
  frag_colour.rgb = color * 0.4 + tex_color.rgb * 0.6;
  frag_colour.a = tex_color.a;
}

