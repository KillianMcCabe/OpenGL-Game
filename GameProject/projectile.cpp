#include "projectile.h"

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

const float max_life_length = 5.0;
const float move_speed = 8.0;

// lantern offset from origin
static const float lantern_x_offset = -0.5;
static const float lantern_z_offset = -1.4;
static const float lantern_y_offset = 2.5;

const float Projectile::width = 0.5;
const float Projectile::height = 0.5;
const float Projectile::depth = 0.5;

static bool initialised = false;

Projectile::Projectile(GLuint shader, float _x, float _y, float _z, vec3 _dir) {
	shader_programme = shader;
	x = _x; y = _y; z = _z;
	life_length = 0;
	dead = false;
	dir = normalize(_dir);
	T = translate(mat4(), vec3(x, y, z));
	R = glm::rotate(glm::mat4(1.0), float(atan2(-dir.x, -dir.z) * 180 / 3.14), glm::vec3(0, 1.0, 0));
	M = T * R;
	init();
}

Projectile::~Projectile() {

}

void Projectile::init()
{
	if (initialised) return; // init once

	shader_programme = load_shaders("simple.vert", "simple.frag");

	// Load objects
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

	bool res;
	res = loadOBJ("assets/projectile.obj", vertices, uvs, normals);
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
	texture = load_texture("assets/projectile_texture.png");

	initialised = true;
}

void Projectile::update(float delta_time)
{
	float dx = delta_time * move_speed * dir.x;
	float dy = delta_time * move_speed * dir.y;
	float dz = delta_time * move_speed * dir.z;

	life_length += delta_time;
	if (life_length > max_life_length) {
		dead = true;
	}
	if (!ObjectManager::collision(x+dx, y+dy, z+dz, width, height, depth)) {
		x += dx;
		y += dy;
		z += dz;
	} else {
		dead = true;
	}
	

	T = translate(mat4(), vec3(x, y, z));
	M = T * R;
}

void Projectile::draw(glm::mat4 V, glm::mat4 P, glm::vec3 light_pos)
{
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