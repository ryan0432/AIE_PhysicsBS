#include "PhysicsObject.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

struct CollisionData
{
	PhysicsObject* obj1;
	PhysicsObject* obj2;

	bool isCollided;
	float overlap;
	glm::vec2 collisionNormal;
};