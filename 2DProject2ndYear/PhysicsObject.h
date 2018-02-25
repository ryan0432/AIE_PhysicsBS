#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>

enum ShapeType
{
	PLANE = 0,
	SPHERE,
	BOX,
	SHAPE_COUNT
};

class PhysicsObject
{
public:
	PhysicsObject() = delete;
	//------ pure virtual function, forces derived class to implement it's own method ------//
	virtual void fixedUpdate(glm::vec2 gravity, float timeStep) = 0;
	virtual void debug() = 0;
	virtual void makeGizmo() = 0;
	ShapeType getShapeID() { return m_shapeID; }
	bool isStatic();

protected:
	PhysicsObject(ShapeType a_shapeID) : m_shapeID(a_shapeID) {};
	//------ The Constructor above is the shortened ver of below ------//
	//
	//PhysicsObject(ShapeType a_shapeID)
	//{
	//	m_shapeID = a_shapeID;
	//}
	//------ Abstract classes don't have constructor (cuz it's virtual) ------//


private:
	ShapeType m_shapeID; //shapeID should be private coz you don't wanna child class
						 //to change it's shape in the half way
};

