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



typedef void(*fn)(CollisionData&);

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

void CollisionManager::plane2Plane(CollisionData& collisionData)
{
	if (collisionData.obj1->isStatic() && collisionData.obj2->isStatic())
	{
		return;
	}
}

void CollisionManager::plane2Sphere(CollisionData& collisionData)
{
	collisionData.swapObjs();
	sphere2Plane(collisionData);
}

void CollisionManager::plane2Box(CollisionData& collisionData)
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
		float smallest = 10;

		//check every corner
		for (auto corner : boxCornersList)
		{
			//check all four corner's dot product with plane normal
			collisionData.overlap = glm::dot( corner, plane->getNormal() ) - plane->getDistanceToOrigin();

			//if the latest check result is less than 1 and then less than previous stored result
			if (collisionData.overlap < smallest)
			{
				//replace smallest with overlap
				smallest = collisionData.overlap;
				collisionData.collisionNormal = plane->getNormal();

				//if the closest corner is overlapping
				if (smallest < 0)
				{
					//negate the overlapping (to make it positive because to caculate
					//resolveCollision and pushApart() needs positive figure)
					collisionData.overlap *= -1;
					collisionData.isCollided = true;
					pushApart(collisionData);
					resolveCollision(collisionData);
				}
			}
		}
		//if the smallest is equal or less than 0, means we have a collision
		collisionData.isCollided = false;
	}
}

void CollisionManager::sphere2Plane(CollisionData& collisionData)
{
	Sphere* sphere = dynamic_cast<Sphere*>(collisionData.obj1);
	Plane* plane = dynamic_cast<Plane*>(collisionData.obj2);

	if (sphere != nullptr && plane != nullptr)
	{
		collisionData.collisionNormal = plane->getNormal();
		collisionData.overlap = glm::dot( sphere->getPosition(), plane->getNormal() ) - plane->getDistanceToOrigin() - sphere->getRadius();

		//if we are behind the plane
		if (collisionData.overlap < 0)
		{
			//negate the overlapping (to make it positive because to caculate
			//resolveCollision and pushApart() needs positive figure)
			collisionData.overlap *= -1;
			collisionData.isCollided = true;
			pushApart(collisionData);
			resolveCollision(collisionData);
		}

		collisionData.isCollided = false;
	}
}

void CollisionManager::sphere2Sphere(CollisionData& collisionData)
{
	//cast objects to sphere vs sphere to see if they are nullptr
	Sphere* sphere1 = dynamic_cast<Sphere*> (collisionData.obj1);
	Sphere* sphere2 = dynamic_cast<Sphere*> (collisionData.obj2);

	//if both objects are not nullptr, we can test for collision
	if (sphere1 != nullptr && sphere2 != nullptr)
	{
		collisionData.collisionNormal = sphere1->getPosition() - sphere2->getPosition();
		float currDis = glm::distance(sphere1->getPosition(), sphere2->getPosition());
		collisionData.overlap = currDis - (sphere1->getRadius() + sphere2->getRadius());

		if (collisionData.overlap < 0)
		{
			collisionData.overlap *= -1;
			collisionData.isCollided = true;
			pushApart(collisionData);
			resolveCollision(collisionData);
		}

		collisionData.isCollided = false;
	}
}

void CollisionManager::sphere2Box(CollisionData& collisionData)
{
	Sphere* sphere = dynamic_cast<Sphere*>(collisionData.obj1);
	Box* box = dynamic_cast<Box*>(collisionData.obj2);

	if (sphere != nullptr && box != nullptr)
	{
		glm::vec2 boxBound = box->getBound();									//         Example:
		glm::vec2 boxCentre = box->getPosition();								//	b1  _______________	 a1
																				//	   |			   |
		glm::vec2 a1(boxCentre.x + boxBound.x, boxCentre.y + boxBound.y);		//	   |	(centre)   |											
		glm::vec2 a2(boxCentre.x + boxBound.x, boxCentre.y - boxBound.y);		//	   |	   * 	   |											
		glm::vec2 b1(boxCentre.x - boxBound.x, boxCentre.y + boxBound.y);		//	   |			   |
		glm::vec2 b2(boxCentre.x - boxBound.x, boxCentre.y - boxBound.y);		//	   |_______________|
																				//	b2					 a2
																				
		glm::vec2 sphereCentre = sphere->getPosition();
		float sphereRadius = sphere->getRadius();
		glm::vec2 collisionPoint = glm::clamp(sphereCentre, b2, a1);
		glm::vec2 collisionNormal = glm::normalize(sphereCentre - collisionPoint);
		float distBetween = glm::distance(sphereCentre, collisionPoint);

		collisionData.collisionNormal = collisionNormal;
		collisionData.overlap = sphereRadius - distBetween;

		//------ debug log ------//
		//std::cout << "collsion normal: (" << collisionData.collisionNormal.x << ", " << collisionData.collisionNormal.y << ")" << std::endl;
		//std::cout << "overlap: " << collisionData.overlap << std::endl;
		//-----------------------//

		if (collisionData.overlap > 0)
		{
			//collisionData.overlap *= -1;
			collisionData.isCollided = true;
			pushApart(collisionData);
			resolveCollision(collisionData);
		}
		else
		{
			collisionData.isCollided = false;
		}
	}
}

void CollisionManager::box2Plane(CollisionData& collisionData)
{
	collisionData.swapObjs();
	plane2Box(collisionData);
}

void CollisionManager::box2Sphere(CollisionData& collisionData)
{
	collisionData.swapObjs();
	sphere2Box(collisionData);
}

void CollisionManager::box2Box(CollisionData& collisionData)
{
	Box* box1 = dynamic_cast<Box*>(collisionData.obj1);
	Box* box2 = dynamic_cast<Box*>(collisionData.obj2);

	std::vector<glm::vec2> box1_cornerList;
	std::vector<glm::vec2> box2_cornerList;
	std::vector<glm::vec2> boxesNormalList;

	if (box1 != nullptr && box2 != nullptr)
	{
		//Box1 Details
		glm::vec2 box1Bound = box1->getBound();												//			Example:
		glm::vec2 box1Centre = box1->getPosition();											//	B1  _______________	 A1  
																							//	   |			   |  
		glm::vec2 box1_A1(box1Centre.x + box1Bound.x, box1Centre.y + box1Bound.y);			//	   |	(centre)   |	  
		glm::vec2 box1_A2(box1Centre.x + box1Bound.x, box1Centre.y - box1Bound.y);			//	   |	   * 	   |	  
		glm::vec2 box1_B1(box1Centre.x - box1Bound.x, box1Centre.y + box1Bound.y);			//	   |	  Box1	   |  
		glm::vec2 box1_B2(box1Centre.x - box1Bound.x, box1Centre.y - box1Bound.y);			//	   |_______________|
																							//	B2					 A2
		box1_cornerList.push_back(box1_A1);
		box1_cornerList.push_back(box1_A2);
		box1_cornerList.push_back(box1_B1);
		box1_cornerList.push_back(box1_B2);

		glm::vec2 box1_vertiNormal = glm::vec2(glm::normalize(box1_A1 - box1_A2).y, -glm::normalize(box1_A1 - box1_A2).x);
		glm::vec2 box1_horizNormal = glm::vec2(-glm::normalize(box1_A1 - box1_B1).y, glm::normalize(box1_A1 - box1_B1).x);

		//Box2 Details
		glm::vec2 box2Bound = box2->getBound();												//			Example:
		glm::vec2 box2Centre = box2->getPosition();											//	B1  _______________	 A1  
																							//	   |			   |  
		glm::vec2 box2_A1(box2Centre.x + box2Bound.x, box2Centre.y + box2Bound.y);			//	   |	(centre)   |	  
		glm::vec2 box2_A2(box2Centre.x + box2Bound.x, box2Centre.y - box2Bound.y);			//	   |	   * 	   |	  
		glm::vec2 box2_B1(box2Centre.x - box2Bound.x, box2Centre.y + box2Bound.y);			//	   |	  Box2	   |  
		glm::vec2 box2_B2(box2Centre.x - box2Bound.x, box2Centre.y - box2Bound.y);			//	   |_______________|
																							//	B2					 A2

		box2_cornerList.push_back(box2_A1);
		box2_cornerList.push_back(box2_A2);
		box2_cornerList.push_back(box2_B1);
		box2_cornerList.push_back(box2_B2);

		glm::vec2 box2_vertiNormal = glm::vec2(glm::normalize(box2_A1 - box2_A2).y, -glm::normalize(box2_A1 - box2_A2).x);
		glm::vec2 box2_horizNormal = glm::vec2(-glm::normalize(box2_A1 - box2_B1).y, glm::normalize(box2_A1 - box2_B1).x);

		boxesNormalList.push_back(box1_vertiNormal);
		boxesNormalList.push_back(box1_horizNormal);
		boxesNormalList.push_back(box2_vertiNormal);
		boxesNormalList.push_back(box2_horizNormal);


		//std::cout << "box1  vertical  normals: (" << box1_vertiNormal.x << ", " << box1_vertiNormal.y << ")" << std::endl;
		//std::cout << "box1 horizontal normals: (" << box1_horizNormal.x << ", " << box1_horizNormal.y << ")" << std::endl;

		//std::cout << "box2  vertical  normals: (" << box2_vertiNormal.x << ", " << box2_vertiNormal.y << ")" << std::endl;
		//std::cout << "box2 horizontal normals: (" << box2_horizNormal.x << ", " << box2_horizNormal.y << ")" << std::endl;

		float minDist_box1 = 200;
		float maxDist_box1 = -200;
		float minDist_box2 = 200;
		float maxDist_box2 = -200;

		float smallestOverlap = 2000;
		glm::vec2 smallestOverlapNormal;

		bool isGap = false;
		//check dot products by edge to corners
		for (auto edge : boxesNormalList)
		{
			for (auto corner : box1_cornerList)
			{
				float dot = glm::dot(corner, edge);
				
				if (dot > maxDist_box1)
				{
					maxDist_box1 = dot;
				}

				if (dot < minDist_box1)
				{
					minDist_box1 = dot;
				}
			}

			for (auto corner : box2_cornerList)
			{
				float dot = glm::dot(corner, edge);

				if (dot > maxDist_box2)
				{
					maxDist_box2 = dot;
				}

				if (dot < minDist_box2)
				{
					minDist_box2 = dot;
				}
			}

			if (maxDist_box1 > minDist_box2 || maxDist_box2 < minDist_box1)
			{
				float overlap1 = maxDist_box1 - minDist_box2;
				float overlap2 = minDist_box1 - maxDist_box2;
				if (std::abs(overlap1) < std::abs(overlap2))
				{
					if (std::abs(overlap1) < std::abs(smallestOverlap))
					{
						smallestOverlap = overlap1;
						smallestOverlapNormal = edge;
					}
				}
				else
				{
					if (std::abs(overlap2) < std::abs(smallestOverlap))
					{
						smallestOverlap = overlap2;
						smallestOverlapNormal = edge;
					}
				}
			}
			else
			{
				isGap = true;
				break;
			}
		}

		if (isGap)
		{
			collisionData.isCollided = false;
		}
		else
		{
			collisionData.isCollided = true;
			collisionData.overlap = -smallestOverlap;
			collisionData.collisionNormal = smallestOverlapNormal;
			pushApart(collisionData);
			resolveCollision(collisionData);
		}

		//std::cout << "overlap: " << smallestOverlap << std::endl;
	}																					  
																						  
}

void CollisionManager::pushApart(CollisionData& collisionData)
{
	//get collisionNormal and overlap form CollisionData
	glm::vec2 collisionNormal = collisionData.collisionNormal;
	float overlap = collisionData.overlap;

	//check if objs are static
	//Case1: if both objs are dynamic (not static), push them apart by their mass ratio
	if ( !(collisionData.obj1->isStatic()) && !(collisionData.obj2->isStatic()) )
	{
		//get instance pointer of two objs
		Rigidbody* obj1 = dynamic_cast <Rigidbody*> (collisionData.obj1);
		Rigidbody* obj2 = dynamic_cast <Rigidbody*> (collisionData.obj2);

		//get two objs' pos for further use
		glm::vec2 obj1Pos = obj1->getPosition();
		glm::vec2 obj2Pos = obj2->getPosition();

		//calculate totall mass
		float totalMass = obj1->getMass() + obj2->getMass();
		//calculate obj1's push ratio by its share in total mass
		float obj1PushRatio = obj1->getMass() / totalMass;
		//calculate obj2's push ratio by its share in total mass
		float obj2PushRatio = obj2->getMass() / totalMass;
		//set both obj's postion by adding push ratio
		obj1->setPosition(obj1Pos + (overlap * obj1PushRatio) * collisionNormal);
		obj2->setPosition(obj2Pos - (overlap * obj2PushRatio) * collisionNormal);
	}

	//Case2: if either one of them is static
	if (collisionData.obj1->isStatic() || collisionData.obj2->isStatic())
	{
		//if obj1 is static
		if (collisionData.obj1->isStatic())
		{
			Rigidbody* obj2 = dynamic_cast <Rigidbody*> (collisionData.obj2);
			glm::vec2 obj2Pos = obj2->getPosition();
			//push obj2 away by full overlap distance
			obj2->setPosition(obj2Pos + overlap * collisionNormal);
		}

		//if obj2 is static
		if (collisionData.obj2->isStatic())
		{
			Rigidbody* obj1 = dynamic_cast <Rigidbody*> (collisionData.obj1);
			glm::vec2 obj1Pos = obj1->getPosition();
			//push obj1 away by full overlap distance
			obj1->setPosition(obj1Pos + overlap * collisionNormal);
		}
	}
}

void CollisionManager::resolveCollision(CollisionData& collisionData)
{
	glm::vec2 collisionNormal = collisionData.collisionNormal;
	//set elasticity to 1 for now. later on maybe set it in CollisionData
	float elasticity = 1;

	//Case1: both objs are dynamic (non-static)
	if (!(collisionData.obj1->isStatic()) && !(collisionData.obj2->isStatic()))
	{
		Rigidbody* obj1 = dynamic_cast<Rigidbody*>(collisionData.obj1);
		Rigidbody* obj2 = dynamic_cast<Rigidbody*>(collisionData.obj2);
		glm::vec2 relativeVelocity = obj2->getVelocity() - obj1->getVelocity();
		float obj1Mass = obj1->getMass();
		float obj2Mass = obj2->getMass();

		float j = glm::dot(-(1 + elasticity) * (relativeVelocity), collisionNormal) /
				  glm::dot(collisionNormal, collisionNormal * (1 / obj1Mass + 1 / obj2Mass));

		glm::vec2 force = collisionNormal * j;
		//apply force to obj2. since addForceToActor() already has opposite force applied to self,
		//we don't need to manually apply force to obj1
		obj1->addForceToActor(obj2, force);
	}

	//Case2: either one of the objs is static
	if (collisionData.obj1->isStatic() || collisionData.obj2->isStatic())
	{
		if (collisionData.obj1->isStatic())
		{
			Rigidbody* obj2 = dynamic_cast<Rigidbody*>(collisionData.obj2);
			float obj2Mass = obj2->getMass();
			glm::vec2 relativeVelocity = obj2->getVelocity();

			float j = glm::dot(-(1 + elasticity) * (relativeVelocity), collisionNormal) /
							  (1 / obj2Mass);

			glm::vec2 force = collisionNormal * j;

			obj2->addForce(force);
		}

		if (collisionData.obj2->isStatic())
		{
			Rigidbody* obj1 = dynamic_cast<Rigidbody*>(collisionData.obj1);
			float obj1Mass = obj1->getMass();
			glm::vec2 relativeVelocity = obj1->getVelocity();

			float j = glm::dot(-(1 + elasticity) * (relativeVelocity), collisionNormal) /
							  (1 / obj1Mass);

			glm::vec2 force = collisionNormal * j;

			obj1->addForce(force);
		}
	}
}