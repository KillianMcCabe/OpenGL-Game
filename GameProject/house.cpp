#include "house.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stb_image.h" // Sean Barrett's image loader
#include "objloader.hpp"
#include "utilities.h"
#include "ObjectManager.h"

static GLuint shader_programme;
static GLuint vao;
static GLuint texture;
static int M_loc;
static int V_loc;
static int P_loc;
static int light_pos_loc;
static int point_count;

const float House::width = 6.5;
const float House::height = 7;
const float House::depth = 5;

House::House() {

}

House::House(float _x, float _y, float _z) {
	x = _x;
	y = _y;
	z = _z;
	M = glm::translate(glm::mat4(1.0), glm::vec3(x, y, z));
}

House::~House() {

}

glm::vec3 House::get_pos() {
	return glm::vec3(x, y, z);
}

static bool initialised = false;
void House::init(GLuint shader)
{
	if (initialised) return; // init once

	shader_programme = shader;

	// Load objects
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	bool res;

	// body
	res = loadOBJ("assets/house.obj", vertices, uvs, normals);
	point_count = vertices.size();
	assert(res);
	std::cout << "loaded assets/house.obj. point count: " << point_count << std::endl;
	vao = create_vao(vertices, uvs, normals);


	// Get uniforms locations
	M_loc = glGetUniformLocation (shader_programme, "M");
	assert (M_loc > -1);
	V_loc = glGetUniformLocation (shader_programme, "V");
	assert (V_loc > -1);
	P_loc = glGetUniformLocation (shader_programme, "P");
	assert (P_loc > -1);
	light_pos_loc = glGetUniformLocation (shader_programme, "light_pos");
	assert (light_pos_loc > -1);


	// Load textures
	texture = load_texture("assets/house_texture.png");

	initialised = true;
}


void House::draw(glm::mat4 V, glm::mat4 P, glm::vec3 light_pos)
{		
	// draw body
	glUseProgram (shader_programme);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glEnable(GL_TEXTURE_2D);

	glUniformMatrix4fv (M_loc, 1, GL_FALSE, &M[0][0]);
	glUniformMatrix4fv (V_loc, 1, GL_FALSE, &V[0][0]);
	glUniformMatrix4fv (P_loc, 1, GL_FALSE, &P[0][0]);
	glUniform3f(light_pos_loc, light_pos[0], light_pos[1], light_pos[2]);

	glBindVertexArray (vao);

	glDrawArrays (GL_TRIANGLES, 0, point_count);
}