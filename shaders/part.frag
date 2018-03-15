#version 330
uniform vec3 cursorPos;
in vec3 pos;
out vec4 frag_colour;

void main () {
	float dist = distance(cursorPos, pos);
	frag_colour = vec4(pos.x, 1.0f - (1.0f / dist) + 0.2f, tan(pos.z), 1.0f);
}
