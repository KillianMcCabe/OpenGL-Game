#ifndef UTILITIES_H
#define UTILITIES_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>


#include "assert.h"
#include "stb_image.h" // Sean Barrett's image loader
#include "objloader.hpp"

using namespace glm;

GLuint create_vao(std::vector<glm::vec3> vertices, std::vector<glm::vec2> uvs, std::vector<glm::vec3> normals);
GLuint load_texture(char * file_loc);
GLuint load_shaders(char * vertex_shader, char * fragment_shader);

#endif