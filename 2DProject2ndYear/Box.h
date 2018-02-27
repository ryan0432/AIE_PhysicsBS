#pragma once
#include "Rigidbody.h"
class Box : public Rigidbody
{
public:
	Box() = delete;
	~Box();
	Box(glm::vec2 position, glm::vec2 velocity,
		float mass, glm::vec2 bound, glm::vec4 colour);


	virtual void makeGizmo() override;
	virtual void debug() override;

	//in this case,Gene recommend to setup the m_position in the centre
	//so bound here is [Centre to Edges]
	glm::vec2 getBound() { return m_bound; }
	glm::vec4 getColour() { return m_colour; }
	

private:
	glm::vec2 m_bound;
	glm::vec4 m_colour;
};

