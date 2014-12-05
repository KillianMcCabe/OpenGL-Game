#include "ObjectManager.h"
#include <algorithm>

static std::vector<Projectile> projectiles;
static std::vector<Tree> trees;
static std::vector<Crow> crows;

static Wizard wizard;

int ObjectManager::crow_shot_count = 0;

ObjectManager::ObjectManager() {
	
}

ObjectManager::~ObjectManager() {

}

void ObjectManager::generateTerrain() {
	int tree_count = 0;

	vec3 pos;
	while (tree_count < 800) {
		int x = RandomFloat(-100.0, 100.0);
		int y = 0;
		int z = RandomFloat(-100.0, 100.0);

		pos = vec3(x, y, z);
		add(Tree(x, y, z));
		tree_count++;
	}
	
}

vec3 ObjectManager::getPlayerPos() {
	return wizard.get_pos();
}

vec3 ObjectManager::getLanternPos() {
	return wizard.getLanternPos();
}

void ObjectManager::setPlayer(Wizard w) {
	wizard = w;
}

void ObjectManager::addProjectile(Projectile p) {
	projectiles.push_back(p);
}

void ObjectManager::add(Tree t) {
	trees.push_back(t);
}

void ObjectManager::add(Crow c) {
	crows.push_back(c);
}



void ObjectManager::clean() {
	for (std::vector<Projectile>::iterator it = projectiles.begin(); it != projectiles.end();) {
		if (it->dead && it != projectiles.end()) {
			it->~Projectile();
			it = projectiles.erase(it);
		} else {
			++it;
		}
	}
	/*
	for (std::vector<Crow>::iterator it = crows.begin(); it != crows.end();) {
		if (it->dead && it != crows.end()) {
			it->~Crow();
			it = crows.erase(it);
		} else {
			++it;
		}
	}
	*/
}

void ObjectManager::update(float delta_time) {
	wizard.update(window, delta_time);
	for (std::vector<Projectile>::iterator it = projectiles.begin(); it != projectiles.end(); ++it) {
		it->update(delta_time);
	}
	for (std::vector<Crow>::iterator it = crows.begin(); it != crows.end(); ++it) {
		it->update(venom.get_pos(), delta_time);
	}
	venom.update(wizard.get_pos(), delta_time);
	clean();
}

void ObjectManager::draw(glm::mat4 V, glm::mat4 P, glm::vec3 light) {
	wizard.draw(V, P, light);
	for (std::vector<Tree>::iterator it = trees.begin(); it != trees.end(); ++it) {
		it->draw(V, P, light);
	}
	for (std::vector<Projectile>::iterator it = projectiles.begin(); it != projectiles.end(); ++it) {
		it->draw(V, P, light);
	}
	for (std::vector<Crow>::iterator it = crows.begin(); it != crows.end(); ++it) {
		it->draw(V, P, light);
	}
	venom.draw(V, P, light);
}

bool ObjectManager::collision(float x, float y, float z, float width, float height, float depth) {
	float x_min = x-width/2;
	float x_max = x+width/2;
	float y_min = y-height/2;
	float y_max = y+height/2;
	float z_min = z-depth/2;
	float z_max = z+depth/2;

	float other_x_min, other_x_max, other_y_min, other_y_max, other_z_min, other_z_max;
	for (std::vector<Tree>::iterator it = trees.begin(); it != trees.end(); ++it) {
		other_x_min = it->x - it->width/2;
		other_x_max = it->x + it->width/2;
		other_y_min = it->y;
		other_y_max = it->y + it->height;
		other_z_min = it->z - it->depth/2;
		other_z_max = it->z + it->depth/2;

		if (x_min < other_x_max && x_max > other_x_min &&
				y_min < other_y_max && y_max > other_y_min &&
				z_min < other_z_max && z_max > other_z_min ){
			return true;
		}
	}

	for (std::vector<Crow>::iterator it = crows.begin(); it != crows.end(); ++it) {
		other_x_min = it->x - it->width/2;
		other_x_max = it->x + it->width/2;
		other_y_min = it->y;
		other_y_max = it->y + it->height;
		other_z_min = it->z - it->depth/2;
		other_z_max = it->z + it->depth/2;

		if (x_min < other_x_max && x_max > other_x_min &&
				y_min < other_y_max && y_max > other_y_min &&
				z_min < other_z_max && z_max > other_z_min ){
			it->dead = true;
			crow_shot_count++;
			return true;
		}
	}


	return false;
}