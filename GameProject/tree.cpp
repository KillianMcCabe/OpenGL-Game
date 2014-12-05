#include "tree.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stb_image.h" // Sean Barrett's image loader
#include "objloader.hpp"
#include "utilities.h"

static GLuint shader_programme;
static GLuint vao;
static GLuint texture;

static int M_loc;
static int V_loc;
static int P_loc;
static int light_pos_loc;

static int point_count;

const float Tree::width = 1;
const float Tree::height = 5;
const float Tree::depth = 1;

Tree::Tree(float _x, float _y, float _z) {
	float r = rand()%360;
	x = _x;
	y = _y;
	z = _z;
	mat4 T = translate(mat4(), vec3(x, y, z));
	mat4 R = rotate(mat4(), r, vec3(0, 1, 0));
	M = T * R;
}

Tree::Tree(GLuint shader, float _x, float _y, float _z) {
	shader_programme = shader;
	float r = rand()%360;
	x = _x;
	y = _y;
	z = _z;
	mat4 T = translate(mat4(), vec3(x, y, z));
	mat4 R = rotate(mat4(), r, vec3(0, 1, 0));
	M = T * R;
	init(shader);
}

Tree::~Tree() {

}

glm::vec3 Tree::get_pos() {
	return glm::vec3(x, y, z);
}

static bool initialised = false;
void Tree::init(GLuint shader)
{
	if (initialised) return; // init once

	shader_programme = shader;

	// Load objects
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

	bool res = loadOBJ("assets/tree2.obj", vertices, uvs, normals);
	point_count = vertices.size();
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
	texture = load_texture("assets/tree_tex.jpg");

	initialised = true;
}

void Tree::draw(glm::mat4 V, glm::mat4 P, glm::vec3 light_pos)
{
	//std::cout << "DRAW " << shader_programme << ", " << texture << ", " << vao << ", " << point_count << ".     " << x  << ", " << y << ", " << z << std::endl;
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