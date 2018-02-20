#include "CollisionManager.h"
#include "PhysicsScene.h"
#include "PhysicsObject.h"
#include "Rigidbody.h"
#include "Plane.h"
#include "Sphere.h"
#include "Box.h"

#include <list>
#include <vector>
#include <algorithm>
#include <iostream>

typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

static fn collisionFunctionArray[] =
{							/*Plane*/						/*Sphere*/							/*Box*/
	/* Plane  */	CollisionManager::plane2Plane,	CollisionManager::plane2Sphere,		CollisionManager::plane2Box,
	/* Sphere */	CollisionManager::sphere2Plane,	CollisionManager::sphere2Sphere,	CollisionManager::sphere2Box,
	/* Box    */	CollisionManager::box2Plane,	CollisionManager::box2Sphere,		CollisionManager::box2Box,
};

void CollisionManager::checkForCollision()
{
	PhysicsScene* physicsScene;
	int actorCount = physicsScene->getActors().size();

	//check collisions against all objects except for self.
	for (int i = 0; i < actorCount - 1; i++)
	{
		for (int j = i + 1; j < actorCount; j++)
		{
			PhysicsObject* object1 = m_actors[i];
			PhysicsObject* object2 = m_actors[j];

			int shapeID1 = object1->getShapeID();
			int shapeID2 = object2->getShapeID();

			//using function pointers; call function pointers by index through the equation below
			int funcIdx = (shapeID1 * SHAPE_COUNT) + shapeID2;

			fn collisionFuncPtr = collisionFunctionArray[funcIdx];

			if (collisionFuncPtr != nullptr)
			{
				collisionFuncPtr(object1, object2);
			}
		}
}
