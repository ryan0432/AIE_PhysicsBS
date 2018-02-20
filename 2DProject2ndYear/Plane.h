#pragma once
#include "PhysicsObject.h"
class Plane : public PhysicsObject
{
public:
	Plane() = delete;
	Plane(glm::vec2 normal, glm::vec4 colour, float distanceToOrigin);
	~Plane();

	//------ inherited from base class, have to override ------// 
	//virtual void fixedUpdate(glm::vec2 gravity, float timeStep) override;
	virtual void fixedUpdate(glm::vec2 gravity, float timeStep) override;
	virtual void debug() override;
	virtual void makeGizmo() override;

	//------ class holds ------//
	///--- now we check collision in PhysicsScene ---///
	//bool checkCollision(PhysicsObject* pOther);
	glm::vec2 getNormal() { return m_normal; }
	float getDistanceToOrigin() { return m_distanceToOrigin; }
	

private:
	glm::vec2 m_normal;
	glm::vec4 m_colour;
	float m_distanceToOrigin;
};

