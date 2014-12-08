#version 400 core

layout (location=0) in vec3 vp; // points
layout (location=1) in vec2 vt; // vertex texture co-ords

out vec2 texture_coordinates;

void main () {
	texture_coordinates = vec2(vt.s, 1-vt.t);
	gl_Position = vec4 (vp, 1.0);
}