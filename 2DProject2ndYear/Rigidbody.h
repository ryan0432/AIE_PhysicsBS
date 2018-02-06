#pragma once
#include "PhysicsObject.h"

class Rigidbody : public PhysicsObject
{
public:
	Rigidbody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity,
		float rotation, float mass);
	~Rigidbody();

	//------ inherited from base class, have to override ------// 
	virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
	virtual void debug();

	//------ pure virtual function, forces derived class to implement it's own method ------//
	virtual bool checkCollision(PhysicsObject* pOther) = 0;

	void addForce(glm::vec2 force);
	void addForceToActor(Rigidbody* actor, glm::vec2 force);

	glm::vec2 getPosition() { return m_position; }
	glm::vec2 getVelocity() { return m_velocity; }
	float getMass() { return m_mass; }
	float getRotation() { return m_rotation; }


private:
	glm::vec2 m_position;
	glm::vec2 m_velocity;
	float m_mass;
	float m_rotation;
};

