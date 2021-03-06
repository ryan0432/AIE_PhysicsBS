#pragma once
#include "Rigidbody.h"
class Sphere : public Rigidbody
{
public:
	Sphere() = delete;
	Sphere(glm::vec2 position, glm::vec2 velocity,
		float mass, float radius, glm::vec4 colour);
	~Sphere();

	virtual void makeGizmo() override;
	virtual void debug() override;

	///--- we do collision in PhysicsScene now ---///
	//virtual bool checkCollision(PhysicsObject* pOther);

	float getRadius() { return m_radius; }

	glm::vec4 getColour() { return m_colour; }

private:
	float m_radius;
	glm::vec4 m_colour;
};

