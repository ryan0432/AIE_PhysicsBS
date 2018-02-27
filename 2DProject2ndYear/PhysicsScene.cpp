#include "PhysicsScene.h"
#include "PhysicsObject.h"
#include "CollisionManager.h"

#include <list>
#include <vector>
#include <algorithm>
#include <iostream>
#include "test.h"

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
	//static std::list<PhysicsObject*> dirty;

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

		CollisionManager::checkForCollision(this);

		//--- checking collision to all actors END ---//

		///------ We don't use old circle collision detection anymore ------//////////////////////////////////
		//for (auto pActor : m_actors)																		//
		//{																									//
		//	for (auto pOther : m_actors)																	//
		//	{																								//
		//		if (pActor == pOther)																		//
		//			continue;																				//
																											//
		//		if (std::find(dirty.begin(), dirty.end(), pActor) != dirty.end() &&							//
		//			std::find(dirty.begin(), dirty.end(), pOther) != dirty.end())							//
		//			continue;																				//
																											//
		//		Rigidbody* pRigid = dynamic_cast<Rigidbody*>(pActor);										//
		//		//set up a saftey guard to make sure pActor is not a nullptr (we actually input an actor)	//
		//		if (pRigid == nullptr)  continue;															//
		//		if (pRigid->checkCollision(pOther) == true)													//
		//		{																							//
		//			pRigid->addForceToActor( dynamic_cast<Rigidbody*>(pOther),								//
		//								   (pRigid->getVelocity() * pRigid->getMass()) );					//
		//			if (pOther == nullptr)  continue;														//
		//																									//
		//			dirty.push_back(pRigid);																//
		//			dirty.push_back(pOther);																//
		//		}																							//
		//	}																								//
		//}																									//
		//dirty.clear();																					//
		///------ We don't use old circle collision detection anymore ------//////////////////////////////////

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
