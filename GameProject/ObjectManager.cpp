#include "ObjectManager.h"
#include <algorithm>

static std::vector<Projectile> projectiles;
static std::vector<Tree> trees;
static std::vector<Crow> crows;

int ObjectManager::crow_shot_count = 0;

ObjectManager::ObjectManager() {
	
}

ObjectManager::~ObjectManager() {

}

void ObjectManager::generateTerrain() {
	std::cout << "generating terrain" << std::endl;
	int tree_count = 0;
	trees.clear();

	int x = RandomFloat(-70.0, 70.0);
	int y = 0;
	int z = RandomFloat(-70.0, 70.0);
	vec3 house_pos = vec3(x, y, z);

	// find suitable house pos
	while ((x < 10 && x > -10) && (z < 10 && z > -10)) {
		x = RandomFloat(-100.0, 100.0);
		y = 0;
		z = RandomFloat(-100.0, 100.0);
	}

	house = House(x, y, z);

	std::cout << "adding trees " << std::endl;
	vec3 pos, diff;
	while (tree_count < 800) {
		x = RandomFloat(-100.0, 100.0);
		y = 0;
		z = RandomFloat(-100.0, 100.0);

		pos = vec3(x, y, z);
		diff = house.get_pos() - pos;
		float distance = sqrtf(dot(diff, diff));

		if (distance > 6 && !((x < 7 && x > -7) && (z < 7 && z > -7))) {
			add(Tree(x, y, z));
			tree_count++;
		}
	}
	std::cout << "generated terrain" << std::endl;
}

void ObjectManager::refresh() {

	std::cout << "refresh" << std::endl;
	// kill the crows
	for (std::vector<Crow>::iterator it = crows.begin(); it != crows.end(); ++it) {
		it->dead = true;
	}

	generateTerrain();
}

vec3 ObjectManager::getPlayerPos() {
	return wizard.get_pos();
}

vec3 ObjectManager::getLanternPos() {
	return wizard.getLanternPos();
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
	house.draw(V, P, light);
	venom.draw(V, P, light);
	for (std::vector<Tree>::iterator it = trees.begin(); it != trees.end(); ++it) {
		it->draw(V, P, light);
	}
	for (std::vector<Projectile>::iterator it = projectiles.begin(); it != projectiles.end(); ++it) {
		it->draw(V, P, light);
	}
	for (std::vector<Crow>::iterator it = crows.begin(); it != crows.end(); ++it) {
		it->draw(V, P, light);
	}
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

	other_x_min = house.x - house.width/2;
	other_x_max = house.x + house.width/2;
	other_y_min = house.y;
	other_y_max = house.y + house.height;
	other_z_min = house.z - house.depth/2;
	other_z_max = house.z + house.depth/2;
	if (x_min < other_x_max && x_max > other_x_min &&
			y_min < other_y_max && y_max > other_y_min &&
			z_min < other_z_max && z_max > other_z_min ){
		house.dead = true;
		return true;
	}


	return false;
}