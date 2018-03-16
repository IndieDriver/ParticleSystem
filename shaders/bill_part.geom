#version 330
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in VertexData {
  vec3 pos;
} vs_data[];

out GeomData {
  vec2 uv;
  vec3 pos;
} gs_data;

void main () {
  vec4 position = gl_in[0].gl_Position;
  gs_data.uv = vec2(0.0f, 0.0f);
  gs_data.pos = vs_data[0].pos;
  gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0);
  EmitVertex();   
  gs_data.uv = vec2(1.0f, 0.0f);
  gl_Position = position + vec4(0.2, -0.2, 0.0, 0.0);
  EmitVertex();
  gs_data.uv = vec2(0.0f, 1.0f);
  gl_Position = position + vec4(-0.2, 0.2, 0.0, 0.0);
  EmitVertex();
  gs_data.uv = vec2(1.0f, 1.0f);
  gl_Position = position + vec4(0.2, 0.2, 0.0, 0.0);
  EmitVertex();

  EndPrimitive();
}
