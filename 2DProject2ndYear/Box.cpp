#include "Box.h"
#include <Gizmos.h>



Box::Box(glm::vec2 position, glm::vec2 velocity,
	float mass, glm::vec2 bound, glm::vec4 colour) : Rigidbody(BOX, position, velocity, 0, mass)
{
}


Box::~Box()
{
}

void Box::makeGizmo()
{
	aie::Gizmos::add2DAABBFilled(m_position, m_bound, m_colour);
}