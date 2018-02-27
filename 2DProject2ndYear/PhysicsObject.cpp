#include "PhysicsObject.h"
#include "Rigidbody.h"

bool PhysicsObject::isStatic()
{
	Rigidbody* rb = dynamic_cast<Rigidbody*>(this);

	if (rb == nullptr)
	{
		return true;
	}

	return false;
}
