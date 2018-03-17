#version 330
layout(location = 0) in vec4 vertex_position;
uniform mat4 MVP;

out VertexData {
  vec3 pos;
} vs_data;

void main () {
  vs_data.pos = vertex_position.xyz;
  gl_Position = MVP * vec4(vertex_position.xyz, 1.0);
}
