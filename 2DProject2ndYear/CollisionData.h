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
	void swapObjs()
	{
		PhysicsObject* tmpObj;
		tmpObj = obj1;
		obj1 = obj2;
		obj2 = tmpObj;
	}
};
