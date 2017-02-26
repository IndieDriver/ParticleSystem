#version 330
in vec3 colour;
out vec4 frag_colour;

void main () {
  frag_colour = vec4 (colour, 1.0);
  //frag_colour = vec4(1.0, 0.0f, 0.0f, 1.0f);
}