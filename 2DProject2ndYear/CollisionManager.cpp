#include "CollisionManager.h"
#include "PhysicsScene.h"
#include "PhysicsObject.h"
#include "Rigidbody.h"
#include "Plane.h"
#include "Sphere.h"
#include "Box.h"
#include "CollisionData.h"

#include <list>
#include <vector>
#include <algorithm>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/ext.hpp>



typedef bool(*fn)(CollisionData&);

fn collisionFunctionArray[] =
{							/*Plane*/						/*Sphere*/							/*Box*/
	/* Plane  */	CollisionManager::plane2Plane,	CollisionManager::plane2Sphere,		CollisionManager::plane2Box,
	/* Sphere */	CollisionManager::sphere2Plane,	CollisionManager::sphere2Sphere,	CollisionManager::sphere2Box,
	/* Box    */	CollisionManager::box2Plane,	CollisionManager::box2Sphere,		CollisionManager::box2Box,
};


void CollisionManager::checkForCollision(PhysicsScene* physicsScene)
{
	//PhysicsScene* physicsScene;
	CollisionData collisionData = CollisionData();
	std::vector<PhysicsObject*> actors = physicsScene->getActors();
	int actorCount = physicsScene->getActors().size();

	//check collisions against all objects except for self.
	for (int i = 0; i < actorCount - 1; i++)
	{
		for (int j = i + 1; j < actorCount; j++)
		{
			PhysicsObject* object1 = actors[i];
			PhysicsObject* object2 = actors[j];

			int shapeID1 = object1->getShapeID();
			int shapeID2 = object2->getShapeID();

			//using function pointers; call function pointers by index through the equation below
			int funcIdx = (shapeID1 * SHAPE_COUNT) + shapeID2;

			fn collisionFuncPtr = collisionFunctionArray[funcIdx];

			if (collisionFuncPtr != nullptr)
			{
				collisionData.obj1 = object1;
				collisionData.obj2 = object2;
				collisionFuncPtr(collisionData);
			}
		}
	}
}

bool CollisionManager::plane2Plane(CollisionData& collisionData)
{
	if (collisionData.obj1->isStatic() && collisionData.obj2->isStatic())
	{
		return true;
	}

	return false;
}

bool CollisionManager::plane2Sphere(CollisionData& collisionData)
{
	collisionData.swapObjs();
	return sphere2Plane(collisionData);
}

bool CollisionManager::plane2Box(CollisionData& collisionData)
{
	std::vector <glm::vec2> boxCornersList;
	Plane* plane = dynamic_cast<Plane*>(collisionData.obj1);
	Box* box = dynamic_cast<Box*>(collisionData.obj2);

	if (plane != nullptr && box != nullptr)
	{
		glm::vec2 boxBound = box->getBound();
		glm::vec2 boxCentre = box->getPosition();

		glm::vec2 a1(boxCentre.x + boxBound.x, boxCentre.y + boxBound.y);		//         Example:
		boxCornersList.push_back(a1);											//	b1  _______________	 a1
		glm::vec2 a2(boxCentre.x + boxBound.x, boxCentre.y - boxBound.y);		//	   |			   |
		boxCornersList.push_back(a2);											//	   |	(centre)   |
		glm::vec2 b1(boxCentre.x - boxBound.x, boxCentre.y + boxBound.y);		//	   |	   * 	   |
		boxCornersList.push_back(b1);				                            //	   |			   |
		glm::vec2 b2(boxCentre.x - boxBound.x, boxCentre.y - boxBound.y);		//	   |_______________|
		boxCornersList.push_back(b2);											//	b2					 a2

		//set the smallet distance to 1,
		//so if the smallest distance is greater than one, it won't calculate
		float smallest = 1;

		//check every corner
		for (auto i : boxCornersList)
		{
			//check all four corner's dot product with plane normal
			collisionData.overlap = glm::dot(i, plane->getNormal());
			//if the latest check result is less than 1 and then less than previous stored result
			if (collisionData.overlap < smallest)
			{
				//replace smallest with overlap
				smallest = collisionData.overlap;
				//collisionData.collisionNormal = 
			}
		}
		//if the smallest is equal or less than 0, means we have a collision
		if (smallest <= 0)
		{
			collisionData.isCollided = true;
			return true;
		}

		collisionData.isCollided = false;
		return false;
	}
}

bool CollisionManager::sphere2Plane(CollisionData& collisionData)
{
	Sphere* sphere = dynamic_cast<Sphere*>(collisionData.obj1);
	Plane* plane = dynamic_cast<Plane*>(collisionData.obj2);

	if (sphere != nullptr && plane != nullptr)
	{
		glm::vec2 collisionNormal = plane->getNormal();
		float sphereToPlane = glm::dot(sphere->getPosition(), plane->getNormal() - plane->getDistanceToOrigin());

		//if we are behind the plane, flip the normal
		if (sphereToPlane < 0)
		{
			collisionNormal *= -1;
			sphereToPlane *= -1;
		}

		float intersection = sphere->getRadius() - sphereToPlane;
		//if two objects tangling with each other
		if (intersection > 0)
		{
			return true;
		}
		return false;
	}
}

bool CollisionManager::sphere2Sphere(CollisionData& collisionData)
{
	//cast objects to sphere vs sphere to see if they are nullptr
	Sphere* sphere1 = dynamic_cast<Sphere*> (collisionData.obj1);
	Sphere* sphere2 = dynamic_cast<Sphere*> (collisionData.obj2);

	//if both objects are not nullptr, we can test for collision
	if (sphere1 != nullptr && sphere2 != nullptr)
	{
		float currDis = glm::distance(sphere1->getPosition(), sphere2->getPosition());
		if (currDis < sphere1->getRadius() + sphere2->getRadius())
		{
			return true;
		}

		return false;
	}
}

bool CollisionManager::sphere2Box(CollisionData& collisionData)
{
	Sphere* sphere = dynamic_cast<Sphere*>(collisionData.obj1);
	Box* box = dynamic_cast<Box*>(collisionData.obj2);
	if (sphere != nullptr && box != nullptr)
	{
		glm::vec2 collisionNormal(box->getPosition() - sphere->getPosition());
		//collision detection START
		//collision detection END
		//-----------------------//
		//collision resolving START
		//collision resolving END

		return true;
	}
	return false;
}

bool CollisionManager::box2Plane(CollisionData& collisionData)
{
	collisionData.swapObjs();
	return plane2Box(collisionData);
}

bool CollisionManager::box2Sphere(CollisionData& collisionData)
{
	collisionData.swapObjs();
	return sphere2Box(collisionData);
}

bool CollisionManager::box2Box(CollisionData& collisionData)
{
	Box* box1 = dynamic_cast<Box*>(collisionData.obj1);
	Box* box2 = dynamic_cast<Box*>(collisionData.obj2);
	if (box1 != nullptr && box2 != nullptr)
	{
		//collision detection START
		//collision detection END
		//-----------------------//
		//collision resolving START
		//collision resolving END
		return true;
	}
	return false;
}

void CollisionManager::pushApart(CollisionData& collisionData)
{
	//get instance pointer of two objs
	Rigidbody* obj1 = (Rigidbody*)collisionData.obj1;
	Rigidbody* obj2 = (Rigidbody*)collisionData.obj2;

	//get two objs' pos for further use
	glm::vec2 obj1Pos = obj1->getPosition();
	glm::vec2 obj2Pos = obj2->getPosition();

	//get collisionNormal and overlap form CollisionData
	glm::vec2 collisionNormal = collisionData.collisionNormal;
	float overlap = collisionData.overlap;

	//check if objs are static
	//Case1: if both objs are dynamic (not static), push them apart by their mass ratio
	if (!(obj1->isStatic()) && !(obj1->isStatic()))
	{
		//calculate totall mass
		float totalMass = obj1->getMass() + obj2->getMass();
		//calculate obj1's push ratio by its share in total mass
		float obj1PushRatio = obj1->getMass() / totalMass;
		//calculate obj2's push ratio by its share in total mass
		float obj2PushRatio = obj2->getMass() / totalMass;
		//set both obj's postion by adding push ratio
		obj1->setPosition(obj1Pos + (overlap * obj1PushRatio) * collisionNormal);
		obj2->setPosition(obj2Pos + (overlap * obj2PushRatio) * collisionNormal);
	}

	//Case2: if either one of them is static
	if (obj1->isStatic() || obj2->isStatic())
	{
		//if obj1 is static
		if (obj1->isStatic())
		{
			//push obj2 away by full overlap distance
			obj2->setPosition(obj2Pos + overlap * collisionNormal);
		}

		//if obj2 is static
		if (obj2->isStatic())
		{
			//push obj1 away by full overlap distance
			obj1->setPosition(obj1Pos + overlap * collisionNormal);
		}
	}
}

void CollisionManager::resolveCollision(CollisionData& collisionData)
{
	Rigidbody* obj1 = (Rigidbody*)collisionData.obj1;
	Rigidbody* obj2 = (Rigidbody*)collisionData.obj2;
	glm::vec2 relativeVelocity = obj2->getVelocity() - obj1->getVelocity();
	glm::vec2 collisionNormal = collisionData.collisionNormal;
	float elasticity = 1;

	//Case1: both objs are dynamic (non-static)
	if (!(obj1->isStatic()) && !(obj2->isStatic()))
	{
		/////////////////////////////////////////////////////////////////////////////////////////////////////////
		// The following line is for getting the colisionNormal, but I will do that in collisionCheck instead. //
		// This is because I do it with CollisionData class.												   //
		// glm::vec2 normal = glm::normalize(actor2->getPosition() - m_position);							   //
		/////////////////////////////////////////////////////////////////////////////////////////////////////////

		float obj1Mass = obj1->getMass();
		float obj2Mass = obj2->getMass();

		float j = glm::dot(-(1 + elasticity) * (relativeVelocity), collisionNormal) /
				  glm::dot(collisionNormal, collisionNormal * (1/obj1Mass + 1/obj2Mass));

		glm::vec2 force = collisionNormal * j;
		//apply force to obj2. since addForceToActor() already has opposite force applied to self,
		//we don't need to manually apply force to obj1
		obj1->addForceToActor(obj2, force);
	}

	//Case2: either one of the objs is static
	if (obj1->isStatic() || obj2->isStatic())
	{

	}
}