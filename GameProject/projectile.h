#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>


class Projectile
{
	private:
		float x, y, z;
		float proj_rot;
		float life_length;
		glm::mat4 M, T, R;
		glm::vec3 dir;
		void init();
	protected:
    public:
		bool dead;
		static const float width, height, depth;
        Projectile(GLuint shader, float _x, float _y, float _z, glm::vec3 _dir);
        ~Projectile();
		void update(float delta_time);
        void draw(glm::mat4 V, glm::mat4 P, glm::vec3 light);
};

#endif