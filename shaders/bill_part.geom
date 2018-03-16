#version 330
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in VertexData {
  vec3 pos;
} vs_data[];

out GeomData {
  vec3 pos;
} gs_data;

void main () {
  vec4 position = gl_in[0].gl_Position;
  gs_data.pos = vs_data[0].pos;
  gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0);
  EmitVertex();   
  gl_Position = position + vec4( 0.2, -0.2, 0.0, 0.0);
  EmitVertex();
  gl_Position = position + vec4(-0.2,  0.2, 0.0, 0.0);
  EmitVertex();
  gl_Position = position + vec4( 0.2,  0.2, 0.0, 0.0);
  EmitVertex();

  EndPrimitive();
}
