#version 400

in vec2 texture_coordinates;
in vec3 vn_cameraspace;
in vec3 light_dir_cameraspace; // direction of the light (from the vertex towards the light)
in vec3 eye_dir_cameraspace; // vector from vertex towards eye/camera

uniform sampler2D basic_texture;

out vec3 out_color;

void main () {
	vec3 light_ambient_colour = vec3 (0.3, 0.3, 0.3);
	vec3 light_diffuse_colour = vec3 (0.8, 0.8, 0.8);
	
	// get texel value
	vec4 texel = texture(basic_texture, texture_coordinates);
	vec3 material_colour = texel.xyz;

	// calculate ambient lighting
	vec3 ambient_lighting = light_ambient_colour * material_colour;

	// calculate diffuse lighting
	float diffuse_intensity = max(0, dot(normalize(-light_dir_cameraspace), normalize(vn_cameraspace)));
	vec3 diffuse_lighting = light_diffuse_colour * material_colour * diffuse_intensity;

	out_color = ambient_lighting + diffuse_lighting;
}
