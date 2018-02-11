#include "Rigidbody.h"
#include <iostream>

Rigidbody::Rigidbody() : PhysicsObject(ShapeType::SPHERE)
{

}

Rigidbody::Rigidbody(ShapeType shapeID, glm::vec2 position, glm::vec2 velocity,
	float rotation, float mass) : PhysicsObject(ShapeType::SPHERE)
{
	m_shapeID = shapeID;
	m_position = position;
	m_velocity = velocity;
	m_mass = mass;
	m_rotation = rotation;
}

Rigidbody::~Rigidbody()
{

}

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
