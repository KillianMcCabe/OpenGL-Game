#include "object.h"



#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stb_image.h" // Sean Barrett's image loader
#include "objloader.hpp"

Object::Object() {
}
Object::Object(char *obj_fileloc, char *tex_fileloc) {

	//
	// Load .obj
	//
	/*
	GLfloat *points = NULL;
	GLfloat *uvs = NULL;
	GLfloat *normals = NULL;
	point_count = 0;
	if(!load_obj_file(obj_fileloc, points, uvs, normals, point_count)) {
		fprintf (stderr, "ERROR: could not find mesh file...\n");
	}
	int uv_count = point_count*3/2;
	int normal_count = point_count;
	*/
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	bool res = loadOBJ(obj_fileloc, vertices, uvs, normals);

	point_count = vertices.size();
	int uv_count = uvs.size();
	int normal_count = normals.size();



	// Set up vertex buffers and vertex array object
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

	//
	// Load texture
	//
	int tex_width = 0;
	int tex_height = 0;
	int components = 0;
	GLubyte *image_data;
	image_data = stbi_load(tex_fileloc, &tex_width, &tex_height, &components, 4);
	printf("%s texture width, height, components: %d, %d, %d\n", tex_fileloc, tex_width, tex_height, components);

	glGenTextures (1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D (GL_TEXTURE_2D,    0, GL_RGBA8,
                tex_width, tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glEnable(GL_TEXTURE_2D);

	//free (points);
	//free (uvs);
	//free (normals);
}

Object::~Object() {

}

void Object::draw(glm::mat4 M, int M_loc)
{		
	// select shader, using same shader
	// glUseProgram (shader_programme);

	// swap texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glEnable(GL_TEXTURE_2D);
		
	// pass uniform variables
	glUniformMatrix4fv (M_loc, 1, GL_FALSE, &M[0][0]);

	// select vao
	glBindVertexArray (vao);

	// draw
	glDrawArrays (GL_TRIANGLES, 0, point_count);

}