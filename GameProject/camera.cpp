
#include "camera.hpp"

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix(){
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix(){
	return ProjectionMatrix;
}

// Initial Field of View
float initialFoV = 45.0f;

float zoom = 0.0f;
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
	zoom += yoffset;
}

void focus_camera_on(vec3 focus, int width, int height){
	float FoV = initialFoV;
	ProjectionMatrix = glm::perspective(FoV, (float)width/(float)height, 0.1f, 100.0f);
	ViewMatrix       = glm::lookAt(focus + vec3(0, 15, -15), focus, vec3(0, 1.0, 0));
}