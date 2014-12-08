#version 400

in vec2 texture_coordinates;
in vec3 vn_cameraspace;
in vec3 light_dir_cameraspace; // direction of the light (from the vertex towards the light)
in vec3 eye_dir_cameraspace; // vector from vertex towards eye/camera
in vec3 pos;

uniform sampler2D basic_texture;
uniform vec3 light_pos;

//out vec3 out_color;
layout(location = 0) out vec3 out_color;
const vec3 fogColor = vec3(0.5,0.5,0.5);

void main () {
	float light_power = 50.0f;
	float light_radius = 10;
	float  spec_exp = 50.0f;

	vec3 light_ambient_colour = vec3 (0.1, 0.1, 0.1);
	vec3 light_diffuse_colour = vec3 (0.6, 0.6, 0.6);
	vec3 light_specular_colour = vec3 (0.5, 0.5, 0.5);

	// Distance to the light
	float distance = length( light_pos - pos );
	
	// get texel value
	vec4 texel = texture(basic_texture, texture_coordinates);
	vec3 material_colour = texel.xyz;

	// calculate ambient lighting
	vec3 ambient_lighting = light_ambient_colour * material_colour;

	// calculate diffuse lighting
	float atten_factor = 1.0/(1 + 2/light_radius*distance + (1/light_radius*light_radius)*distance*distance);
	float diffuse_intensity = max(0, dot(normalize(-light_dir_cameraspace), normalize(vn_cameraspace)));
	vec3 diffuse_lighting = light_diffuse_colour * material_colour * diffuse_intensity * light_power * atten_factor;//light_power / (distance*distance);

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
	//fogFactor = 1.0 /exp(dist * FogDensity);
	fogFactor = 1.0 /exp( (dist * FogDensity)* (dist * FogDensity));
	fogFactor = clamp( fogFactor, 0.0, 1.0 );
	

	//fogFactor = 1 - fogFactor;
	//out_color = vec3( fogFactor, fogFactor, fogFactor);

	out_color = mix(fogColor, finalColor, fogFactor);
}
