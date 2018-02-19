#include "PhysicsScene.h"
#include "PhysicsObject.h"
#include "Rigidbody.h"
#include "Sphere.h"
#include "Plane.h"
#include <list>
#include <algorithm>
#include <iostream>
#include "test.h"


//------ check collision section START ------//
typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

static fn collisionFunctionArray[]=
{
	PhysicsScene::plane2Plane,	PhysicsScene::plane2Sphere,
	PhysicsScene::sphere2Plane,	PhysicsScene::sphere2Sphere,
};

void PhysicsScene::checkForCollision()
{
	int actorCount = m_actors.size();

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
				collisionFuncPtr(object1, object2);
		}
	}
}

bool PhysicsScene::plane2Plane(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Plane* plane1 = dynamic_cast<Plane*>(obj1);
	Plane* plane2 = dynamic_cast<Plane*>(obj2);

	if (plane1 == nullptr && plane2 == nullptr)
		true;
	else
		false;
}

bool PhysicsScene::plane2Sphere(PhysicsObject* obj1, PhysicsObject* obj2) // funcIdx: 1
{
	sphere2Plane(obj2, obj1);
}

bool PhysicsScene::sphere2Plane(PhysicsObject* obj1, PhysicsObject* obj2) // funcIdx: 2
{
	Sphere* sphere = dynamic_cast<Sphere*>(obj1);
	Plane* plane = dynamic_cast<Plane*>(obj2);

	if (sphere != nullptr && plane != nullptr)
	{
		glm::vec2 collisionNormal = plane->getNormal();
		float sphereToPlane = glm::dot( sphere->getPosition(), plane->getNormal() - plane->getDistanceToOrigin() );

		//if we are behind the plane, flip the normal
		if (sphereToPlane < 0)
		{
			collisionNormal *= -1;
			sphereToPlane *= -1;
		}

		float intersection = sphere->getRadius() - sphereToPlane;
		//if tow objects tangling with each other
		if (intersection > 0)
		{
			//set sphere velocity to zero
			sphere->addForce(-sphere->getVelocity()); // in = out, have to fix later
			return true;
		}
		return false;
	}
}

bool PhysicsScene::sphere2Sphere(PhysicsObject* obj1, PhysicsObject* obj2) // funcIdx: 3
{
	//cast objects to sphere vs sphere to see if they are nullptr
	Sphere* sphere1 = dynamic_cast<Sphere*> (obj1);
	Sphere* sphere2 = dynamic_cast<Sphere*> (obj2);

	//if both objects are not nullptr, we can test for collision
	if (sphere1 != nullptr && sphere2 != nullptr)
	{
		float currDis = glm::distance(sphere1->getPosition(), sphere2->getPosition());
		if (currDis < sphere1->getRadius() + sphere2->getRadius())
			return true;
		else
			return false;
	}
}

//------ check collision section END ------//

PhysicsScene::PhysicsScene() : m_timeStep(0.01f), m_gravity(glm::vec2(0, 0))
{
}


PhysicsScene::~PhysicsScene()
{
	for (auto pActor : m_actors)
	{
		delete pActor;
	}
}

void PhysicsScene::addActor(PhysicsObject* actor)
{
	assert(actor != nullptr);
	assert(std::find( std::begin(m_actors), std::end(m_actors), actor ) == std::end(m_actors));
	m_actors.push_back(actor);
}

void PhysicsScene::removeActor(PhysicsObject* actor)
{
	m_actors.erase(std::remove(m_actors.begin(), m_actors.end(), actor ), m_actors.end());
}

void PhysicsScene::update(float deltaTime)
{
	static std::list<PhysicsObject*> dirty;

	// update physics at a fixed time step
	static float accumulatedTime = 0.0f;
	accumulatedTime += deltaTime;

	while (accumulatedTime >= m_timeStep)
	{
		for (auto pActor : m_actors)
		{
			pActor->fixedUpdate(m_gravity, m_timeStep);
		}

		accumulatedTime -= m_timeStep;

		//--- checking collision to all actors START ---//
		for (auto pActor : m_actors)
		{
			for (auto pOther : m_actors)
			{
				if (pActor == pOther)
					continue;

				if (std::find(dirty.begin(), dirty.end(), pActor) != dirty.end() &&
					std::find(dirty.begin(), dirty.end(), pOther) != dirty.end())
					continue;

				Rigidbody* pRigid = dynamic_cast<Rigidbody*>(pActor);
				//set up a saftey guard to make sure pActor is not a nullptr (we actually input an actor)
				if (pRigid == nullptr)  continue;
				if (pRigid->checkCollision(pOther) == true)
				{
					pRigid->addForceToActor( dynamic_cast<Rigidbody*>(pOther), (pRigid->getVelocity() * pRigid->getMass()) );
					if (pOther == nullptr)  continue;

					dirty.push_back(pRigid);
					dirty.push_back(pOther);
				}
			}
		}
		dirty.clear();
		//--- checking collision to all actors END ---//
	}

	//////////////////////////////
	// namespace usage example  //
	// MathHelp::Power(2.0f,3); //
	//////////////////////////////
}

void PhysicsScene::updateGizmos()
{
	for (auto pActor : m_actors)
	{
		pActor->makeGizmo();
	}
}

void PhysicsScene::debugScene()
{
	int count = 0;
	for (auto pActor : m_actors)
	{
		std::cout << count << " : ";
		pActor->debug();
		count++;
	}
}
