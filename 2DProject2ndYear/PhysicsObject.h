#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>

enum ShapeType
{
	PLANE = 0,
	SPHERE,
	BOX
};

class PhysicsObject
{
public:
	//------ pure virtual function, forces derived class to implement it's own method ------//
	virtual void fixedUpdate(glm::vec2 gravity, float timeStep) = 0;
	virtual void debug() = 0;
	virtual void makeGizmo() = 0;
	virtual void resetPosition() {};

protected:
	ShapeType m_shapeID;
	PhysicsObject(ShapeType a_shapeID) : m_shapeID(a_shapeID) {};
	//------ Constructor above is the shortened ver of below ------//
	//The
	//PhysicsObject(ShapeType a_shapeID)
	//{
	//	m_shapeID = a_shapeID;
	//}
	//------ Abstract classes don't have constructor (cuz it's virtual) ------//
};

