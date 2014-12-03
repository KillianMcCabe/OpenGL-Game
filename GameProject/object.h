#ifndef OBJECT_H
#define OBJECT_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

class Object
{
	private:
		GLuint vao;
		GLuint texture;
		int point_count;
	protected:
    public:
		Object();
        Object(char * obj_fileloc, char * tex_fileloc);
        ~Object();
        void draw(glm::mat4 M, int M_loc);
};

#endif