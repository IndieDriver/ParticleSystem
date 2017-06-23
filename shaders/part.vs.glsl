#version 330
layout(location = 0) in vec4 vertex_position;
layout(location = 1) in vec4 vertex_colour;
uniform mat4 MVP;
out vec3 colour;
out vec3 pos;
out vec3 cursorPos;

void main () {
	pos = vertex_position.rgb;
	colour = vertex_colour.xyz;
	gl_Position = MVP * vec4(vertex_position.xyz, 1.0);
}
