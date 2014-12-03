#ifndef WIZARD_H
#define WIZARD_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>


class Wizard
{
	private:
		glm::mat4 body_M, lantern_hand_M, staff_hand_M;
		glm::mat4 body_R, body_T;
		glm::mat4 lantern_hand_local_T, lantern_hand_local_R;
		glm::mat4 staff_hand_local_T, staff_hand_local_R;
		float reload_time;
		float x, y, z;
		float distance_moved;
		glm::vec3 facing_direction;
		glm::vec3 moving_direction;
		static void init();
	protected:
    public:
        Wizard(GLuint shader);
        ~Wizard();
		glm::vec3 get_pos();
		void update(GLFWwindow* window, float delta_time);
		glm::vec3 getLanternPos();
        void draw(glm::mat4 V, glm::mat4 P, glm::vec3 light);
};

#endif