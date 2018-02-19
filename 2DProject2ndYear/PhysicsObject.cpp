#include "PhysicsObject.h"
#include "Rigidbody.h"

bool PhysicsObject::isStatic()
{
	Rigidbody* rb = dynamic_cast<Rigidbody*>(this);

	return rb == nullptr;
}
