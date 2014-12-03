#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>

using namespace glm;

class Fountain
{
	private:

		static void init();
	protected:
    public:
        Fountain();
		~Fountain();
		void draw(float delta, mat4 V, mat4 P, vec3 light_pos);
};

#endif