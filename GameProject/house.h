#ifndef HOUSE_H
#define HOUSE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

class House
{
	private:
		glm::mat4 M;
		glm::mat4 T, R;
	protected:
    public:
		bool dead;
		float x, y, z;
		float distance_moved;
		static const float width, height, depth;
		static void init(GLuint shader);
		House();
        House(float x, float y, float z);
        ~House();
		glm::vec3 get_pos();
        void draw(glm::mat4 V, glm::mat4 P, glm::vec3 light);
};

#endif