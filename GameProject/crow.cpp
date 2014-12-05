#include "crow.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stb_image.h" // Sean Barrett's image loader
#include "objloader.hpp"
#include "utilities.h"

static const float move_speed = 4.5;
static const float crow_wing_speed = 3.0;

static GLuint shader_programme;
static GLuint body_vao;
static GLuint left_wing_vao;
static GLuint right_wing_vao;
static GLuint body_texture;
static GLuint wing_texture;
static int M_loc;
static int V_loc;
static int P_loc;
static int light_pos_loc;
static int body_point_count;
static int left_wing_point_count;
static int right_wing_point_count;

const float Crow::width = 1.2;
const float Crow::height = 0.5;
const float Crow::depth = 1;




Crow::Crow(GLuint shader, float _x, float _y, float _z) {
	shader_programme = shader;
	dead = false;
	distance_moved = 0;
	flying_dir = glm::vec3(rand_int(-1, 1), 0, rand_int(-1, 1));
	x = _x;
	y = _y;
	z = _z;
	init();
}

Crow::~Crow() {

}

static bool initialised = false;
void::Crow::init()
{
	if (initialised) return; // init once

	// Load objects
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	int point_count, uv_count, normal_count;
	bool res;
	// body
	res = loadOBJ("assets/crow_body.obj", vertices, uvs, normals);
	point_count = vertices.size();
	uv_count = uvs.size();
	normal_count = normals.size();
	body_point_count = point_count;
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
		
		glGenVertexArrays (1, &body_vao);
		glBindVertexArray (body_vao);

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
	// left wing
	vertices.clear();
	uvs.clear();
	normals.clear();
	res = loadOBJ("assets/crow_left_wing.obj", vertices, uvs, normals);
	point_count = vertices.size();
	uv_count = uvs.size();
	normal_count = normals.size();
	left_wing_point_count = point_count;
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
		
		glGenVertexArrays (1, &left_wing_vao);
		glBindVertexArray (left_wing_vao);

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
	// right wing
	vertices.clear();
	uvs.clear();
	normals.clear();
	res = loadOBJ("assets/crow_right_wing.obj", vertices, uvs, normals);
	point_count = vertices.size();
	uv_count = uvs.size();
	normal_count = normals.size();
	right_wing_point_count = point_count;
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
		
		glGenVertexArrays (1, &right_wing_vao);
		glBindVertexArray (right_wing_vao);

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
	int tex_width = 0;
	int tex_height = 0;
	int components = 0;
	GLubyte *image_data;
	// body
	image_data = stbi_load("assets/crow_body_texture.png", &tex_width, &tex_height, &components, 4);
	printf("%s texture width, height, components: %d, %d, %d\n", "crow_body_texture", tex_width, tex_height, components);
	glGenTextures (1, &body_texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, body_texture);
	glTexImage2D (GL_TEXTURE_2D,    0, GL_RGBA8,
                tex_width, tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glEnable(GL_TEXTURE_2D);

	//wing
	tex_width = 0;
	tex_height = 0;
	components = 0;
	image_data = stbi_load("assets/crow_wing_texture.png", &tex_width, &tex_height, &components, 4);
	printf("%s texture width, height, components: %d, %d, %d\n", "crow_wing_texture", tex_width, tex_height, components);
	glGenTextures (1, &wing_texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, wing_texture);
	glTexImage2D (GL_TEXTURE_2D,    0, GL_RGBA8,
                tex_width, tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glEnable(GL_TEXTURE_2D);

	initialised = true;
}

void::Crow::update(glm::vec3 target, float delta_time)
{
	if (dead) {
		// reset position
		x = rand_int(x, x+100);
		z = rand_int(z, z+100);
		dead = false;
	} else {
		//glm::vec3 turn_towards = target;
		vec3 new_flying_dir = target - glm::vec3(x, y, z);
		float distance = sqrtf(dot(new_flying_dir, new_flying_dir));
		if (distance < 3) {
			venom.dead = true;
		}

		flying_dir = normalize(new_flying_dir);
		

		float dx = delta_time * move_speed * flying_dir.x;
		float dz = delta_time * move_speed * flying_dir.z;
		x += dx;
		z += dz;

		distance_moved += delta_time;
	}

	float crow_wing_angle = sin(distance_moved * crow_wing_speed) * 15;

	// move body
	crow_body_T = glm::translate(glm::mat4(1.0), glm::vec3(x, y, z));
	crow_body_R = glm::rotate(glm::mat4(1.0), float(atan2(-flying_dir.x, -flying_dir.z) * 180 / 3.14), glm::vec3(0, 1.0, 0));
	crow_body_M = crow_body_T * crow_body_R;

	// move left wing
	crow_left_wing_local_T = glm::translate(glm::mat4(1.0), glm::vec3(std::max(crow_wing_angle/500*move_speed, 0.0f), 0.0, 0.0));
	crow_left_wing_local_R = glm::rotate(glm::mat4(1.0), crow_wing_angle, glm::vec3(0.0, 0.0, 1.0));
	crow_left_wing_M = crow_body_M * crow_left_wing_local_T * crow_left_wing_local_R;

	// move right wing
	crow_right_wing_local_T = glm::translate(glm::mat4(1.0), glm::vec3(std::min(-crow_wing_angle/500*move_speed, 0.0f), 0.0, 0.0));
	crow_right_wing_local_R = glm::rotate(glm::mat4(1.0), -crow_wing_angle, glm::vec3(0.0, 0.0, 1.0));
	crow_right_wing_M = crow_body_M * (crow_right_wing_local_T * crow_right_wing_local_R);
}

void::Crow::update(float delta_time)
{
	z += -float(delta_time * move_speed);
	if (z <= -20) {
		z += 30;
	}

	float crow_wing_angle = sin(z * crow_wing_speed) * 15;

	// move body
	crow_body_M = glm::translate(glm::mat4(1.0), glm::vec3(x, y, z));

	// move left wing
	crow_left_wing_local_T = glm::translate(glm::mat4(1.0), glm::vec3(std::max(crow_wing_angle/500*move_speed, 0.0f), 0.0, 0.0));
	crow_left_wing_local_R = glm::rotate(glm::mat4(1.0), crow_wing_angle, glm::vec3(0.0, 0.0, 1.0));
	crow_left_wing_M = crow_body_M * crow_left_wing_local_T * crow_left_wing_local_R;

	// move right wing
	crow_right_wing_local_T = glm::translate(glm::mat4(1.0), glm::vec3(std::min(-crow_wing_angle/500*move_speed, 0.0f), 0.0, 0.0));
	crow_right_wing_local_R = glm::rotate(glm::mat4(1.0), -crow_wing_angle, glm::vec3(0.0, 0.0, 1.0));
	crow_right_wing_M = crow_body_M * (crow_right_wing_local_T * crow_right_wing_local_R);
}

void Crow::draw(glm::mat4 V, glm::mat4 P, glm::vec3 light_pos)
{		

	// draw body
	glUseProgram (shader_programme);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, body_texture);
	glEnable(GL_TEXTURE_2D);

	glUniformMatrix4fv (M_loc, 1, GL_FALSE, &crow_body_M[0][0]);
	glUniformMatrix4fv (V_loc, 1, GL_FALSE, &V[0][0]);
	glUniformMatrix4fv (P_loc, 1, GL_FALSE, &P[0][0]);
	glUniform3f(light_pos_loc, light_pos[0], light_pos[1], light_pos[2]);

	glBindVertexArray (body_vao);

	glDrawArrays (GL_TRIANGLES, 0, body_point_count);


	// draw left wing
	//glUseProgram (shader_programme);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, wing_texture);
	glEnable(GL_TEXTURE_2D);

	glUniformMatrix4fv (M_loc, 1, GL_FALSE, &crow_left_wing_M[0][0]);

	glBindVertexArray (left_wing_vao);

	glDrawArrays (GL_TRIANGLES, 0, left_wing_point_count);


	// draw right wing
	//glUseProgram (shader_programme);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, wing_texture);
	glEnable(GL_TEXTURE_2D);

	glUniformMatrix4fv (M_loc, 1, GL_FALSE, &crow_right_wing_M[0][0]);

	glBindVertexArray (right_wing_vao);

	glDrawArrays (GL_TRIANGLES, 0, right_wing_point_count);

}