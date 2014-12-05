#version 400

layout (location=0) in vec3 vp; // points
layout (location=1) in vec2 vt; // vertex texture co-ords
layout (location=2) in vec3 vn; // normals

uniform mat4 M; // model matrix
uniform mat4 V; 
uniform mat4 P; 
uniform vec3 light_pos;

out vec2 texture_coordinates;
out vec3 vn_cameraspace;
out vec3 light_dir_cameraspace;
out vec3 eye_dir_cameraspace;
out vec3 pos;

void main () {
	texture_coordinates = vec2(vt.x, vt.y); // invert

	pos = (M * vec4(vp, 1)).xyz;

	vec3 vp_cameraspace = (V * M * vec4(vp, 1.0)).xyz;
	eye_dir_cameraspace = vec3(0,0,0) - vp_cameraspace;   // vector from vertex towards eye/camera

	// normal moved to cameraspace
	vn_cameraspace = (V * M * vec4(vn, 0)).xyz;

	vec3 light_pos_cameraspace = (V * vec4(light_pos, 1.0)).xyz;
	light_dir_cameraspace = vp_cameraspace - light_pos_cameraspace; // direction of the light (from the light towards the vertex)

	gl_Position = P * V * M * vec4 (vp, 1.0);
}
