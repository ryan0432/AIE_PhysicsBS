#pragma once
class PhysicsScene;
class PhysicsObject;
class Rigidbody;
class Plane;
class Sphere;
class Box;
struct CollisionData;

class CollisionManager
{
public:
	CollisionManager() = delete;
	~CollisionManager() = delete;

	static void checkForCollision(PhysicsScene* physicsScene);

	static void plane2Plane(CollisionData&);
	static void plane2Sphere(CollisionData&);
	static void plane2Box(CollisionData&);
	static void sphere2Plane(CollisionData&);
	static void sphere2Sphere(CollisionData&);
	static void sphere2Box(CollisionData&);
	static void box2Plane(CollisionData&);
	static void box2Sphere(CollisionData&);
	static void box2Box(CollisionData&);

	//this function pushes apart tanling objects caused by timestep difference
	static void pushApart(CollisionData& collisionData);

	//the resolution for two colliding objects
	static void resolveCollision(CollisionData& collisionData);

};

