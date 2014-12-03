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

#include "projectile.h"
#include "tree.h"
#include "crow.h"


class ObjectManager
{
	private:
		void clean();
	protected:
    public:
        ObjectManager();
        ~ObjectManager();
		void static addProjectile(Projectile p);
		void add(Tree t);
		void add(Crow c);
		void update(float delta_time);
		bool static collision(float x, float y, float z, float width, float height, float depth);
        void draw(glm::mat4 V, glm::mat4 P, glm::vec3 light);
};

#endif