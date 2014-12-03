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


class Venom
{
	private:
		glm::mat4 M;
		glm::mat4 T, R;
		glm::vec3 moving_dir;
		static void init();
	protected:
    public:
		bool dead;
		float x, y, z;
		float distance_moved;
		static const float width, height, depth;
        Venom(GLuint shader, float x, float y, float z);
        ~Venom();
		void update(glm::vec3 target, float delta_time);
        void draw(glm::mat4 V, glm::mat4 P, glm::vec3 light);
};

#endif