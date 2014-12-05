#version 400

in vec2 texture_coordinates;

uniform sampler2D input_texture;

out vec4 frag_colour;

void main () {
	vec4 texel = texture(input_texture, texture_coordinates);
	frag_colour = texel;
}
