#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

class PhysicsObject;
class Rigidbody;
class PhysicsScene
{
public:
	PhysicsScene();
	~PhysicsScene();

	//struct CollisionData
	//{

	//};

	void addActor(PhysicsObject* actor);
	void removeActor(PhysicsObject* actor);
	void update(float dt);
	void updateGizmos();
	void debugScene();

	void setGravity(const glm::vec2 gravity) { m_gravity = gravity; }
	glm::vec2 getGravity() const { return m_gravity; }

	void setTimeStep(const float timeStep) { m_timeStep = timeStep; }
	float getTimeStep() const { return m_timeStep; }

	std::vector<PhysicsObject*> getActors() const { return m_actors; }

	//------ check collision section START ------//
	void checkForCollision();
																// func index
	static bool plane2Plane(PhysicsObject*, PhysicsObject*);	// 0
	static bool plane2Sphere(PhysicsObject*, PhysicsObject*);	// 1
	static bool sphere2Plane(PhysicsObject*, PhysicsObject*);	// 2
	static bool sphere2Sphere(PhysicsObject*, PhysicsObject*);	// 3
	//------ check collision section END ------//



protected:
	glm::vec2 m_gravity;
	float m_timeStep;
	std::vector<PhysicsObject*> m_actors;

};

