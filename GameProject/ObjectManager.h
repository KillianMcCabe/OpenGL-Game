#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "wizard.h"
#include "projectile.h"
#include "tree.h"
#include "crow.h"
#include "house.h"
#include "venom.h"
#include "utilities.h"


extern GLFWwindow* window;
extern Wizard wizard;
extern Venom venom;
extern House house;

class ObjectManager
{
	private:
		void clean();
	protected:
    public:
		static int crow_shot_count;
        ObjectManager();
        ~ObjectManager();
		void static addProjectile(Projectile p);
		vec3 getPlayerPos();
		vec3 getLanternPos();
		void generateTerrain();
		void add(Tree t);
		void add(Crow c);
		
		void update(float delta_time);
		bool static collision(float x, float y, float z, float width, float height, float depth);
		void refresh();
        void draw(glm::mat4 V, glm::mat4 P, glm::vec3 light);
};

#endif