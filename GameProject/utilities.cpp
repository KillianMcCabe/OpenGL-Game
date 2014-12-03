
#include "utilities.h"

using namespace glm;


glm::vec3 TransformDirection(glm::vec3 pDirection, glm::mat4 pMatrix){
	glm::vec4 result = pMatrix * glm::vec4(pDirection, 0.0f);
    return glm::vec3(result.x, result.y, result.z);
}

float RandomFloat(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

int rand_int(int min, int max) {
	return rand()%(max-min)+min;
}


//
// copy a shader from a plain text file into a character array
bool parse_file_into_str (
	const char* file_name, char* shader_str, int max_len
) {
	FILE* file = fopen (file_name , "r");
	int current_len = 0;
	char line[2048];

	shader_str[0] = '\0'; /* reset string */
	if (!file) {
		fprintf (stderr, "ERROR: opening file for reading: %s\n", file_name);
		return false;
	}
	strcpy (line, ""); /* remember to clean up before using for first time! */
	while (!feof (file)) {
		if (NULL != fgets (line, 2048, file)) {
			current_len += strlen (line); /* +1 for \n at end */
			if (current_len >= max_len) {
				fprintf (stderr, 
					"ERROR: shader length is longer than string buffer length %i\n",
					max_len
				);
			}
			strcat (shader_str, line);
		}
	}
	if (EOF == fclose (file)) { /* probably unnecesssary validation */
		fprintf (stderr, "ERROR: closing file from reading %s\n", file_name);
		return false;
	}
	return true;
}

GLuint create_vao(std::vector<glm::vec3> vertices, std::vector<glm::vec2> uvs, std::vector<glm::vec3> normals) {
	GLuint vao;

	int point_count, uv_count, normal_count;
	point_count = vertices.size();
	uv_count = uvs.size();
	normal_count = normals.size();
	{
		GLuint points_vbo, uvs_vbo, normals_vbo;

		glGenBuffers (1, &points_vbo);
		glBindBuffer (GL_ARRAY_BUFFER, points_vbo);
		glBufferData (GL_ARRAY_BUFFER, point_count*sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);		
		glGenBuffers (1, &uvs_vbo);
		glBindBuffer (GL_ARRAY_BUFFER, uvs_vbo);
		glBufferData (GL_ARRAY_BUFFER, uv_count*sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
		glGenBuffers (1, &normals_vbo);
		glBindBuffer (GL_ARRAY_BUFFER, normals_vbo);
		glBufferData (GL_ARRAY_BUFFER, normal_count*sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
		
		glGenVertexArrays (1, &vao);
		glBindVertexArray (vao);

		glEnableVertexAttribArray (0);
		glBindBuffer (GL_ARRAY_BUFFER, points_vbo);
		glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray (1);
		glBindBuffer (GL_ARRAY_BUFFER, uvs_vbo);
		glVertexAttribPointer (1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray (2);
		glBindBuffer (GL_ARRAY_BUFFER, normals_vbo);
		glVertexAttribPointer (2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	}

	return vao;
}

GLuint load_texture(char * file_loc) {
	GLuint Texture;

	int tex_width = 0;
	int tex_height = 0;
	int components = 0;
	GLubyte *image_data;

	image_data = stbi_load(file_loc, &tex_width, &tex_height, &components, 4);
	std::printf("%s texture width, height, components: %d, %d, %d\n", file_loc, tex_width, tex_height, components);
	glGenTextures (1, &Texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glTexImage2D (GL_TEXTURE_2D,    0, GL_RGBA8,
                tex_width, tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glEnable(GL_TEXTURE_2D);

	return Texture;
}

GLuint load_shaders(char * vertex_shader, char * fragment_shader) {
	GLuint shader_programme;
	{
		char* vertex_shader_str;
		char* fragment_shader_str;
		
		// allocate some memory to store shader strings
		vertex_shader_str = (char*)malloc (81920);
		fragment_shader_str = (char*)malloc (81920);
		// load shader strings from text files
		assert (parse_file_into_str (vertex_shader, vertex_shader_str, 81920));
		assert (parse_file_into_str (fragment_shader, fragment_shader_str, 81920));
		GLuint vs, fs;
		vs = glCreateShader (GL_VERTEX_SHADER);
		fs = glCreateShader (GL_FRAGMENT_SHADER);
		glShaderSource (vs, 1, (const char**)&vertex_shader_str, NULL);
		glShaderSource (fs, 1, (const char**)&fragment_shader_str, NULL);
		// free memory
		free (vertex_shader_str);
		free (fragment_shader_str);
		glCompileShader (vs);
		glCompileShader (fs);
		shader_programme = glCreateProgram ();
		glAttachShader (shader_programme, fs);
		glAttachShader (shader_programme, vs);
		glLinkProgram (shader_programme);
		/* TODO NOTE: you should check for errors and print logs after compiling and also linking shaders */
	}
	return shader_programme;
}