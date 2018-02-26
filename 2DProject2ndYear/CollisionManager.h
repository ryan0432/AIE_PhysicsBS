#pragma once
class PhysicsScene;
class PhysicsObject;
class Rigidbody;
class Plane;
class Sphere;
class Box;
class CollisionData;

class CollisionManager
{
public:
	CollisionManager() = delete;
	~CollisionManager() = delete;

	static void checkForCollision(PhysicsScene* physicsScene);

	static bool plane2Plane(CollisionData&);
	static bool plane2Sphere(CollisionData&);
	static bool plane2Box(CollisionData&);
	static bool sphere2Plane(CollisionData&);
	static bool sphere2Sphere(CollisionData&);
	static bool sphere2Box(CollisionData&);
	static bool box2Plane(CollisionData&);
	static bool box2Sphere(CollisionData&);
	static bool box2Box(CollisionData&);

	//this function pushes apart tanling objects caused by timestep difference
	static void pushApart(CollisionData& collisionData);

	//the resolution for two colliding objects
	static void resolveCollision(CollisionData& collisionData);

};

