#include "venom.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stb_image.h" // Sean Barrett's image loader
#include "objloader.hpp"
#include "utilities.h"
#include "ObjectManager.h"

static const float move_speed = 3.0;
static const float follow_distance = 1.5;

static GLuint shader_programme;
static GLuint vao;
static GLuint texture;
static int M_loc;
static int V_loc;
static int P_loc;
static int light_pos_loc;
static int point_count;

const float Venom::width = 1.2;
const float Venom::height = 0.5;
const float Venom::depth = 1;

Venom::Venom() {
	dead = false;
	distance_moved = 0;
	dead_time = 0;
	moving_dir = glm::vec3(rand_int(-1, 1), 0, rand_int(-1, 1));
}

Venom::Venom(GLuint shader, float _x, float _y, float _z) {
	shader_programme = shader;
	dead = false;
	home = false;
	distance_moved = 0;
	dead_time = 0;
	moving_dir = glm::vec3(rand_int(-1, 1), 0, rand_int(-1, 1));
	x = _x;
	y = _y;
	z = _z;
	init();
}

Venom::~Venom() {

}

glm::vec3 Venom::get_pos() {
	return glm::vec3(x, y, z);
}

static bool initialised = false;
void Venom::init()
{
	if (initialised) return; // init once

	// Load objects
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	bool res;

	// body
	res = loadOBJ("assets/venom.obj", vertices, uvs, normals);
	point_count = vertices.size();
	assert(res);
	std::cout << "loaded assets/venom.obj. point count: " << point_count << std::endl;
	vao = create_vao(vertices, uvs, normals);
	std::cout << "loaded assets/venom.obj. vao: " << vao << std::endl;


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
	texture = load_texture("assets/venom_tex.png");
	std::cout << "loaded assets/venom.obj. texture: " << texture << std::endl;

	initialised = true;
}

void Venom::update(glm::vec3 target, float delta_time)
{
	if (dead) {
		T = glm::translate(glm::mat4(1.0), glm::vec3(x, y+0.5, z));
		R = glm::rotate(glm::mat4(1.0), 90.0f, glm::vec3(x, 0.0, 0));
		dead_time += delta_time;
		if (dead_time >= 5.0) {
			game_over = true;
		}
	} else if (home) {
	} else {
		vec3 house_pos = house.get_pos();
		vec3 dif = house_pos - glm::vec3(x, y, z);
		float distance = sqrtf(dot(dif, dif));
		if (distance < 5) {
			x = house_pos.x;
			y = house_pos.y;
			z = house_pos.z;
			venom.home = true;
			game_win = true;
		}

		moving_dir = normalize(target - glm::vec3(x, y, z));
	
		vec3 diff = target - vec3(x, y, z);
			distance = sqrtf(dot(diff, diff));

			if (distance >= follow_distance) {
				float dx = delta_time * move_speed * moving_dir.x;
				float dz = delta_time * move_speed * moving_dir.z;
	
				if (!ObjectManager::collision(x+dx, y, z, width, height, depth)) {
					x += dx;
				}
				if (!ObjectManager::collision(x, y, z+dz, width, height, depth)) {
					z += dz;
				}

				distance_moved += delta_time;

				
			}

			T = glm::translate(glm::mat4(1.0), glm::vec3(x, y, z));
			R = glm::rotate(glm::mat4(1.0), float(atan2(moving_dir.x, moving_dir.z) * 180 / 3.14), glm::vec3(0, 1.0, 0));
	}
	
	
	M = T * R;
}


void Venom::draw(glm::mat4 V, glm::mat4 P, glm::vec3 light_pos)
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