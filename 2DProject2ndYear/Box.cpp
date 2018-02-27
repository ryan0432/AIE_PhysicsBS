#include "Box.h"
#include <Gizmos.h>
#include <iostream>



Box::Box(glm::vec2 position, glm::vec2 velocity,
	float mass, glm::vec2 bound, glm::vec4 colour) : Rigidbody(BOX, position, velocity, 0, mass), m_bound(bound), m_colour(colour)
{
}


Box::~Box()
{
}

void Box::makeGizmo()
{
	aie::Gizmos::add2DAABBFilled(m_position, m_bound, m_colour);
}

void Box::debug()
{
	std::cout << "Box width:  " << m_bound.x * 2 << std::endl;
	std::cout << "Box height: " << m_bound.y * 2 << std::endl;
}