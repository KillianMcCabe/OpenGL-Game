#ifndef CAMERA_HPP
#define CAMERA_HPP

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

void focus_camera_on(vec3 focus, int width, int height);
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();

#endif