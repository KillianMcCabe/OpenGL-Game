#version 400

in vec2 texture_coordinates;
in vec3 vn_cameraspace;
in vec3 light_dir_cameraspace; // direction of the light (from the vertex towards the light)
in vec3 eye_dir_cameraspace; // vector from vertex towards eye/camera

uniform sampler2D basic_texture;

out vec3 out_color;
const vec3 fogColor = vec3(0.5,0.5,0.5);

void main () {
	float  spec_exp = 50.0f;
	vec3 light_ambient_colour = vec3 (0.3, 0.3, 0.3);
	vec3 light_diffuse_colour = vec3 (0.8, 0.8, 0.8);
	vec3 light_specular_colour = vec3 (0.5, 0.5, 0.5);
	
	// get texel value
	vec4 texel = texture(basic_texture, texture_coordinates);
	vec3 material_colour = texel.xyz;

	// calculate ambient lighting
	vec3 ambient_lighting = light_ambient_colour * material_colour;

	// calculate diffuse lighting
	float diffuse_intensity = max(0, dot(normalize(-light_dir_cameraspace), normalize(vn_cameraspace)));
	vec3 diffuse_lighting = light_diffuse_colour * material_colour * diffuse_intensity;

	// calculate specular lighting
	vec3 r = reflect(normalize(light_dir_cameraspace), normalize(vn_cameraspace));
	vec3 v = normalize(eye_dir_cameraspace); // vector towards viewer
	vec3 specular_lighting = light_specular_colour * vec3 (1.0, 1.0, 1.0) * pow (max(0, dot (r, v)), spec_exp);

	//vec3 finalColor = ambient_lighting + diffuse_lighting + specular_lighting;
	vec3 finalColor = ambient_lighting + diffuse_lighting;

	vec3 c = vec3(0,0,0);
	float dist =0 ;
	float fogFactor  = 0;

	dist = (gl_FragCoord.z / gl_FragCoord.w);

	const float FogDensity = 0.05;
	fogFactor = 1.0 /exp(dist * FogDensity);
	fogFactor = clamp( fogFactor, 0.0, 1.0 );
	

	//fogFactor = 1 - fogFactor;
	//out_color = vec3( fogFactor, fogFactor, fogFactor);

	out_color = mix(fogColor, finalColor, fogFactor);
}
