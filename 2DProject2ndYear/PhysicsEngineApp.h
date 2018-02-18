#pragma once
#include "Application.h"
#include "Renderer2D.h"

class PhysicsScene;
class PhysicsEngineApp : public aie::Application
{
public:
	PhysicsEngineApp();
	~PhysicsEngineApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;
	PhysicsScene*		m_physicsScene;

private:
	float keyTimer = 0.0f;
	bool emissionTimer(float deltaTime, float emissionRate);
	float rocketTotallMass = 100;
	float rocketShellMass = 5.0f;
	float rocketRadius = 10;
	float gasMass = 5.0f;
	float gasRadius = 3.0f;

	void debugLog(float deltaTime);

};

