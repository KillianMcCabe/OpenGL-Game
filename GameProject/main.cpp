
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "text.h"
#include "object.h"
#include "camera.hpp"
#include "utilities.h"

#include "wizard.h"
#include "crow.h"
#include "venom.h"
#include "fountain.h"
#include "tree.h"
#include "projectile.h"
#include "ObjectManager.h"

// files to use for font. change path here
const char* atlas_image = "freemono.png";
const char* atlas_meta = "freemono.meta";

// Globals
GLFWwindow* window = NULL;
bool game_over = false;
Venom venom;
int crow_shot_count;

//
// dimensions of the window drawing surface
int gl_width = 1024;
int gl_height = 768;

glm::mat4 P, V, M;

using namespace glm;

int main () {
	const GLubyte* renderer;
	const GLubyte* version;

	//
	// Start OpenGL using helper libraries
	// --------------------------------------------------------------------------
	if (!glfwInit ()) {
		fprintf (stderr, "ERROR: could not start GLFW3\n");
		return 1;
	} 

	/* change to 3.2 if on Apple OS X
	glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); */

	window = glfwCreateWindow (gl_width, gl_height, "Hello Spooky", NULL, NULL);
	if (!window) {
		fprintf (stderr, "ERROR: opening OS window\n");
		return 1;
	}
	glfwMakeContextCurrent (window);
	
	glewExperimental = GL_TRUE;
	glewInit ();

	glfwSetCursorPos(window, gl_width/2, gl_height/2);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	/* get version info */
	renderer = glGetString (GL_RENDERER); /* get renderer string */
	version = glGetString (GL_VERSION); /* version as a string */
	printf ("Renderer: %s\n", renderer);
	printf ("OpenGL version supported %s\n", version);

	//GLuint simple_shader_programme = load_shaders("simple.vert", "simple.frag");
	GLuint simple_shader_programme = load_shaders("fog_shader.vert", "fog_shader.frag");
	GLuint quad_shader = load_shaders("quad.vert", "quad.frag");
	
	GLuint game_over_screen_texture = load_texture("assets/game_over_screen.jpg");

	// Get uniform locations
	int M_loc = glGetUniformLocation (simple_shader_programme, "M");
	assert (M_loc > -1);
	int V_loc = glGetUniformLocation (simple_shader_programme, "V");
	assert (V_loc > -1);
	int P_loc = glGetUniformLocation (simple_shader_programme, "P");
	assert (P_loc > -1);
	int light_pos_loc = glGetUniformLocation (simple_shader_programme, "light_pos");
	assert (light_pos_loc > -1);

	glUseProgram (simple_shader_programme);

	// Pass light position to shader
	vec3 light_pos = vec3(8.0, 5.0, -8.5);
	glUniform3f(light_pos_loc, light_pos[0], light_pos[1], light_pos[2]);
	
	//
	// Start rendering
	// --------------------------------------------------------------------------
	// tell GL to only draw onto a pixel if the fragment is closer to the viewer
	glEnable (GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc (GL_LESS); // depth-testing interprets a smaller value as "closer"
	glDisable(GL_CULL_FACE);

	ObjectManager objects = ObjectManager();
	Tree::init(simple_shader_programme);
	objects.generateTerrain();

	// add grass to scene
	Object ground = Object("assets/ground.obj", "assets/grass_tex.png");

	// add trees to scene
	//objects.add(Tree(simple_shader_programme, -3.0, 0.0, 3.0));
	//objects.add(Tree(simple_shader_programme, 3.0, 0.0, 3.0));
	

	// add crow
	objects.add(Crow(simple_shader_programme, rand_int(-100, 100), 2.5, rand_int(0, 100)));
	objects.add(Crow(simple_shader_programme, rand_int(-100, 100), 2.5, rand_int(0, 100)));
	objects.add(Crow(simple_shader_programme, rand_int(-100, 100), 2.5, rand_int(0, 100)));
	objects.add(Crow(simple_shader_programme, rand_int(-100, 100), 2.5, rand_int(0, 100)));
	objects.add(Crow(simple_shader_programme, rand_int(-100, 100), 2.5, rand_int(0, 100)));
	objects.add(Crow(simple_shader_programme, rand_int(-100, 100), 2.5, rand_int(0, 100)));

	//objects.add(Venom(simple_shader_programme, rand_int(-5, 5), 0, rand_int(-5, 5)));

	Wizard wizard = Wizard(simple_shader_programme);
	objects.setPlayer(wizard);

	venom = Venom(simple_shader_programme, rand_int(-5, 5), 0, rand_int(-5, 5));

	Fountain fountain = Fountain();


	assert (init_text_rendering (atlas_image, atlas_meta, gl_width, gl_height));
	int crow_count_text_id = add_text (
		"Crows shot: ",
		-0.95f, -0.8f, 50.0f, 1.0f, 1.0f, 1.0f, 1.0f);

	focus_camera_on(vec3(0, 0, 0), gl_width, gl_height);
	V = getViewMatrix();
	P = getProjectionMatrix();
	
	//
	// create vao for rendering image
	//
	// The fullscreen quad's FBO
	static const GLfloat g_quad_vertex_buffer_data[] = { 
		-1.0f,  1.0f,  0.0f,
	     1.0f, -1.0f,  0.0f,
	    -1.0f, -1.0f,  0.0f,
	  
	    -1.0f,  1.0f,  0.0f,
	     1.0f,  1.0f,  0.0f,
	     1.0f, -1.0f,  0.0f
	};

	GLfloat tex_uvs[] = {
		0.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f
	};

	GLuint quad_vertexbuffer;
	GLuint uvs_vbo;
	GLuint quad_vao;
	glGenBuffers(1, &quad_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);
	glGenBuffers (1, &uvs_vbo);
	glBindBuffer (GL_ARRAY_BUFFER, uvs_vbo);
	glBufferData (GL_ARRAY_BUFFER, sizeof (tex_uvs), tex_uvs, GL_STATIC_DRAW);

	glGenVertexArrays (1, &quad_vao);
	glBindVertexArray (quad_vao);

	glEnableVertexAttribArray (0);
	glBindBuffer (GL_ARRAY_BUFFER, quad_vertexbuffer);
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray (1);
	glBindBuffer (GL_ARRAY_BUFFER, uvs_vbo);
	glVertexAttribPointer (1, 2, GL_FLOAT, GL_FALSE, 0, NULL);



	double lastTime = glfwGetTime();
	double currentTime;

	while (!glfwWindowShouldClose (window)) {
		if (glfwGetKey(window, GLFW_KEY_ESCAPE)== GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
		glClearColor(0.5, 0.5, 0.5, 1);//gray color, same as fog color

		currentTime = glfwGetTime();
		float deltaTime = float(currentTime - lastTime);

		if (game_over) {

			// Use our shader
			glUseProgram(quad_shader);

			// Bind our texture textures
			GLuint texLoc = glGetUniformLocation(quad_shader, "input_texture");
			assert(texLoc != -1);
			glUniform1i(texLoc, 0);

			glActiveTexture(GL_TEXTURE0);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, game_over_screen_texture);

			// select vao
			glBindVertexArray (quad_vao);

			glDrawArrays(GL_TRIANGLES, 0, 6);

			if (glfwGetKey(window, GLFW_KEY_SPACE)== GLFW_PRESS) {
				game_over = false;
				venom = Venom(simple_shader_programme, rand_int(-5, 5), 0, rand_int(-5, 5));
				wizard = Wizard(simple_shader_programme);
				objects.setPlayer(wizard);
				ObjectManager::crow_shot_count = 0;

				//objects.refresh(); TODO
			}
			
		} else {

			focus_camera_on(objects.getPlayerPos(), gl_width, gl_height);
			V = getViewMatrix();
			P = getProjectionMatrix();

			glUniformMatrix4fv (V_loc, 1, GL_FALSE, &V[0][0]);
			glUniformMatrix4fv (P_loc, 1, GL_FALSE, &P[0][0]);

			light_pos = objects.getLanternPos();
			glUniform3f(light_pos_loc, light_pos[0], light_pos[1], light_pos[2]);

			glUseProgram (simple_shader_programme);
			ground.draw(mat4(1.0), M_loc);
			//fountain.draw(deltaTime, V, P, light_pos);

			objects.update(deltaTime);
			objects.draw(V, P, light_pos);


			char tmp[256];
			sprintf (tmp, "Crows shot: %d\n", ObjectManager::crow_shot_count);
			update_text (crow_count_text_id, tmp);
			draw_texts();
		}
		lastTime = currentTime;
		/* this just updates window events and keyboard input events (not used yet) */
		glfwPollEvents ();
		/* swaps the buffer that we are drawing to, and the one currently displayed on the window */
		glfwSwapBuffers (window);
	}

	return 0;
}
