#version 330
layout(location = 0) in vec4 vertex_position;
layout(location = 1) in vec4 vertex_colour;
uniform mat4 MVP;
out vec3 colour;

void main () {
  colour = vertex_colour.xyz;
  //colour = vec3(1.0f, 0.0f, 0.0);
  gl_Position = MVP * vec4(vertex_position.xyz, 1.0);
  //gl_Position = vec4(vertex_position, 1.0);
   //gl_Position = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}