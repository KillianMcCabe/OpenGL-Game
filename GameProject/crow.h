#ifndef CROW_H
#define CROW_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>


class Crow
{
	private:
		

		glm::mat4 crow_body_M, crow_left_wing_M, crow_right_wing_M;
		glm::mat4 crow_left_wing_local_T, crow_left_wing_local_R;
		glm::mat4 crow_right_wing_local_T, crow_right_wing_local_R;

		static void init();
	protected:
    public:
		bool dead;
		float x, y, z;
		static const float width, height, depth;
        Crow(GLuint shader, float x, float y, float z);
        ~Crow();
		void update(float delta_time);
        void draw(glm::mat4 V, glm::mat4 P, glm::vec3 light);
};

#endif