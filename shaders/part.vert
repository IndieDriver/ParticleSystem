#version 330
layout(location = 0) in vec4 vertex_position;
uniform mat4 MVP;
out vec3 pos;
out vec3 cursorPos;

void main () {
	pos = vertex_position.xyz;
	gl_Position = MVP * vec4(vertex_position.xyz, 1.0);
}
