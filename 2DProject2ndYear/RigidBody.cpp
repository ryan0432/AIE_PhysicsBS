#include "Rigidbody.h"
#include <iostream>

void Rigidbody::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	addForce(gravity * m_mass * timeStep);
	m_position += m_velocity * timeStep;
}

void Rigidbody::debug()
{
	std::cout << "Rigidbody Object\n" << std::endl;
}

void Rigidbody::addForce(glm::vec2 force)
{
	glm::vec2 acc;
	acc = force / m_mass; //[F = m * a] Therefore [a = F / m]
	m_velocity += acc;
}

void Rigidbody::addForceToActor(Rigidbody* actor, glm::vec2 force)
{
	actor->addForce(force);

	addForce(-force);
}
