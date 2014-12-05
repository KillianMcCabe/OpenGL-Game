#ifndef VENOM_H
#define VENOM_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "house.h"

extern House house;
extern bool game_over;
extern bool game_win;

class Venom
{
	private:
		glm::mat4 M;
		glm::mat4 T, R;
		glm::vec3 moving_dir;
		float dead_time;
		static void init();
	protected:
    public:
		bool dead;
		bool home;
		float x, y, z;
		float distance_moved;
		static const float width, height, depth;
		Venom();
        Venom(GLuint shader, float x, float y, float z);
        ~Venom();
		glm::vec3 get_pos();
		void update(glm::vec3 target, float delta_time);
        void draw(glm::mat4 V, glm::mat4 P, glm::vec3 light);
};

#endif