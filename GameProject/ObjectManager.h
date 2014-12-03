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
#include "venom.h"

extern GLFWwindow* window;

class ObjectManager
{
	private:
		void clean();
	protected:
    public:
        ObjectManager();
        ~ObjectManager();
		void static addProjectile(Projectile p);
		vec3 getPlayerPos();
		void setPlayer(Wizard w);
		void add(Tree t);
		void add(Crow c);
		void add(Venom v);
		void update(float delta_time);
		bool static collision(float x, float y, float z, float width, float height, float depth);
        void draw(glm::mat4 V, glm::mat4 P, glm::vec3 light);
};

#endif