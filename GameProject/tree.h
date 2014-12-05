#ifndef TREE_H
#define TREE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace glm;

class Tree
{
	private:
		mat4 M;
	protected:
    public:
		float x, y, z;
		static const float width;
		static const float height;
		static const float depth;
		glm::vec3 get_pos();
		static void init(GLuint shader);
		Tree(float x, float y, float z);
        Tree(GLuint shader, float x, float y, float z);
        ~Tree();
        void draw(glm::mat4 V, glm::mat4 P, glm::vec3 light);
};

#endif