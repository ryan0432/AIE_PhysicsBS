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

void Sphere::makeGizmo(glm::vec2& centre, float radius, unsigned int segments,
					   glm::vec4 colour, glm::mat4* transfrom)
{
	aie::Gizmos::add2DCircle(centre, radius, segments, colour, transfrom);
}

bool Sphere::checkCollision(PhysicsObject* pOther)
{
	//if( ((Sphere*)pOther)->getRadius() - )
	
}
