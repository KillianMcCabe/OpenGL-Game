#include "wizard.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stb_image.h" // Sean Barrett's image loader
#include "objloader.hpp"
#include "utilities.h"

#include "ObjectManager.h"
#include "projectile.h"

static GLuint shader_programme;
static GLuint body_vao;
static GLuint lantern_hand_vao;
static GLuint staff_hand_vao;
static GLuint body_texture;
static GLuint lantern_hand_texture;
static GLuint staff_hand_texture;
static int M_loc;
static int V_loc;
static int P_loc;
static int light_pos_loc;
static int body_point_count;
static int lantern_hand_point_count;
static int staff_hand_point_count;

static const float move_speed = 4.0;
static const float turn_speed = 0.8;
static const float fire_rate = 0.3;
static const float lantern_bobble_speed = 8.0;

// lantern offset from origin
static const float lantern_x_offset = -0.5;
static const float lantern_z_offset = -1.4;
static const float lantern_y_offset = 2.5;

// staff offset from origin
static const float staff_x_offset = -0.07;
static const float staff_y_offset = 2.5;
static const float staff_z_offset = 0.68;

// staff offset from origin
static const float body_front_x_offset = -0.07;
static const float body_front_y_offset = 2.0;
static const float body_front_z_offset = 0.68;

static const float width = 1;
static const float height = 3;
static const float depth = 1;

ObjectManager objects = ObjectManager();

Wizard::Wizard() {
	//shader_programme = load_shaders("simple.vert", "simple.frag");
	x = 0; y = 0; z = 0;
	reload_time = 0;
	facing_direction = vec3(0, 0, 1);
	distance_moved = 0.0;
	//init();
}

Wizard::Wizard(GLuint shader) {
	shader_programme = shader;
	x = 0; y = 0; z = 0;
	reload_time = 0;
	facing_direction = vec3(0, 0, 1);
	distance_moved = 0.0;
	init();
}

Wizard::~Wizard() {

}

glm::vec3 Wizard::get_pos() {
	return glm::vec3(x, y, z);
}

void Wizard::init()
{

	// Load objects
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	int point_count, uv_count, normal_count;
	bool res;

	// body
	res = loadOBJ("assets/wizard_body.obj", vertices, uvs, normals);
	body_point_count = vertices.size();
	body_vao = create_vao(vertices, uvs, normals);

	// lantern hand
	vertices.clear();
	uvs.clear();
	normals.clear();
	res = loadOBJ("assets/wizard_lantern_hand.obj", vertices, uvs, normals);
	lantern_hand_point_count = vertices.size();
	lantern_hand_vao = create_vao(vertices, uvs, normals);
	
	// staff hand
	vertices.clear();
	uvs.clear();
	normals.clear();
	res = loadOBJ("assets/wizard_staff_hand.obj", vertices, uvs, normals);
	staff_hand_point_count = vertices.size();
	staff_hand_vao = create_vao(vertices, uvs, normals);


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
	body_texture = load_texture("assets/wizard_body_texture.png");
	lantern_hand_texture = load_texture("assets/wizard_lantern_hand_texture.png");
	staff_hand_texture = load_texture("assets/wizard_staff_hand_texture.png");
}

glm::vec3 Wizard::getLanternPos() {
	return glm::vec3(lantern_hand_M[3][0]+lantern_x_offset, lantern_hand_M[3][1]+lantern_y_offset, lantern_hand_M[3][2]+lantern_z_offset);
}

void::Wizard::update(GLFWwindow* window, float delta_time)
{
	moving_direction = vec3(0, 0, 0);
	
	// Move up
	if (glfwGetKey(window, GLFW_KEY_UP ) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_W ) == GLFW_PRESS){
		moving_direction.z += 1;
	}
	// Move down
	if (glfwGetKey(window, GLFW_KEY_DOWN ) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S ) == GLFW_PRESS){
		moving_direction.z -= 1;
	}
	// Move left
	if (glfwGetKey(window, GLFW_KEY_LEFT ) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A ) == GLFW_PRESS){
		moving_direction.x += 1;
	}
	// move right
	if (glfwGetKey(window, GLFW_KEY_RIGHT ) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D ) == GLFW_PRESS){
		moving_direction.x -= 1;
	}

	float dz = delta_time * move_speed * moving_direction.z;
	float dx = delta_time * move_speed * moving_direction.x;
	if (!objects.collision(x+dx, y, z, width, height, depth)) {
		x += dx;
	}
	if (!objects.collision(x, y, z+dz, width, height, depth)) {
		z += dz;
	}

	// calculate body matrix
	body_T = glm::translate(glm::mat4(), glm::vec3(x, y, z));
	if (moving_direction.z != 0 || moving_direction.x != 0) { 
		body_R = glm::rotate(glm::mat4(1.0), float(atan2(moving_direction.x, moving_direction.z) * 180 / 3.14), glm::vec3(0, 1.0, 0));
		facing_direction = moving_direction;
		distance_moved += delta_time;
	} else {
		distance_moved = 0;
	}

	// fire projectile
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS && reload_time <= 0){
		//ObjectManager::addProjectile(Projectile(shader_programme, x+staff_x_offset+facing_direction.x, y+staff_y_offset+facing_direction.y, z+staff_z_offset+facing_direction.z, facing_direction));
		double mouse_x, mouse_y;
		glfwGetCursorPos(window, &mouse_x, &mouse_y);

		
		vec3 screen_centre = vec3(gl_width/2, 0, gl_height/2-20);
		vec3 mouse_click = vec3(mouse_x, 0, mouse_y);
		vec3 towards_mouse_click = normalize(screen_centre - mouse_click);
		
		/*
		int mouse_depth = 0;
		glReadPixels(mouse_x, gl_height - mouse_y - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &mouse_depth);

		glm::vec4 viewport = glm::vec4(0, 0, gl_width, gl_height);
		glm::vec3 wincoord = glm::vec3(mouse_x, mouse_y, mouse_depth);
		glm::vec3 objcoord = glm::unProject(wincoord, V, P, viewport);
		vec3 towards_mouse_click = normalize(objcoord - vec3(x, y, z));
		*/

		ObjectManager::addProjectile(Projectile(shader_programme, x+towards_mouse_click.x, y+staff_y_offset, z+towards_mouse_click.z, towards_mouse_click));

		reload_time = fire_rate;
	} else {
		reload_time -= delta_time;
	}
	
	body_M =  body_T * body_R;

	// calculate lantern hand matrix
	lantern_hand_local_T = glm::translate(glm::mat4(1.0), glm::vec3(0.0, sin(distance_moved*lantern_bobble_speed)/20, 0.0));
	lantern_hand_M = body_M * lantern_hand_local_T;

	// calculate staff hand matrix
	staff_hand_local_T = glm::mat4(1.0);
	staff_hand_M = body_M * staff_hand_local_T;
}

void Wizard::draw(glm::mat4 V, glm::mat4 P, glm::vec3 light_pos)
{
	// draw body
	glUseProgram (shader_programme);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, body_texture);
	glEnable(GL_TEXTURE_2D);

	glUniformMatrix4fv (M_loc, 1, GL_FALSE, &body_M[0][0]);
	glUniformMatrix4fv (V_loc, 1, GL_FALSE, &V[0][0]);
	glUniformMatrix4fv (P_loc, 1, GL_FALSE, &P[0][0]);
	glUniform3f(light_pos_loc, light_pos[0], light_pos[1], light_pos[2]);

	glBindVertexArray (body_vao);

	glDrawArrays (GL_TRIANGLES, 0, body_point_count);


	// draw lantern hand
	//glUseProgram (shader_programme);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, lantern_hand_texture);
	glEnable(GL_TEXTURE_2D);

	glUniformMatrix4fv (M_loc, 1, GL_FALSE, &lantern_hand_M[0][0]);

	glBindVertexArray (lantern_hand_vao);

	glDrawArrays (GL_TRIANGLES, 0, lantern_hand_point_count);


	// draw staff hand
	//glUseProgram (shader_programme);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, staff_hand_texture);
	glEnable(GL_TEXTURE_2D);

	glUniformMatrix4fv (M_loc, 1, GL_FALSE, &staff_hand_M[0][0]);

	glBindVertexArray (staff_hand_vao);

	glDrawArrays (GL_TRIANGLES, 0, staff_hand_point_count);

}