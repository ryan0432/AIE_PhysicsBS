#include "Plane.h"
#include <iostream>
#include <Gizmos.h>



Plane::Plane(glm::vec2 normal, glm::vec4 colour, float distanceToOrigin) : PhysicsObject(PLANE)
{
	m_normal = normal;
	m_distanceToOrigin = distanceToOrigin;
	m_colour = colour;
}


Plane::~Plane()
{

}



void Plane::makeGizmo()
{
	float lineExtensionLength = 300;
	glm::vec2 centrePoint = m_normal * m_distanceToOrigin;
	glm::vec2 parallel(m_normal.y, -m_normal.x);
	glm::vec2 lineStart = centrePoint + (parallel * lineExtensionLength);
	glm::vec2 lindEnd = centrePoint - (parallel * lineExtensionLength);

	aie::Gizmos::add2DLine(lineStart, lindEnd, m_colour);
}