#include "Sphere.h"
#include <Gizmos.h>



Sphere::Sphere(glm::vec2 position, glm::vec2 velocity,
	float mass, float radius, glm::vec4 colour) : Rigidbody(SPHERE, position, velocity, 0, mass)
{
	m_radius = radius;
	m_colour = colour;
}


Sphere::~Sphere()
{

}

void Sphere::makeGizmo()
{
	//Gizmos::add2DCircle <- this function, if not do extra scaling or rotating, don't have to input transform
	aie::Gizmos::add2DCircle(m_position, m_radius, 32U, m_colour );
	//basically we have got enough information that inherited from all base and derived parent classes to draw the shape
	//Such as m_position is from Rigidbody class, m_radius and m_colour from self (Sphere) class
}

bool Sphere::checkCollision(PhysicsObject* pOther)
{
	float currDistance = glm::distance( getPosition(), ((Sphere*)pOther)->getPosition() );
	float collisionRange = ( (Sphere*)pOther )->getRadius() + m_radius;
	
	if (currDistance < collisionRange)
		return true;
	else
		false;
}

