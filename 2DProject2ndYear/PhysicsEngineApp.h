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
#pragma region Rocket Test
	////------ Rocket Test ------//
	//float keyTimer = 0.0f;
	//bool emissionTimer(float deltaTime, float emissionRate);
	//float rocketTotallMass = 100;
	//float rocketShellMass = 5.0f;
	//float rocketRadius = 10;
	//float gasMass = 5.0f;
	//float gasRadius = 3.0f;
#pragma endregion

	//------ ImGUI Section ------//
	bool isMainWindowOpen = true;
	float inputColour[4] = {0.5f, 0.5f, 0.5f, 1.0f};
	int currShapeType = 0;
;
	float initialVelocity[2] = {0.0f, 0.0f};
	float initialMass = 0.1f;
	float initialSphereRadius = 1.0f;
	float initialBoxDemention[2] = {1.0f, 1.0f};
	float initialPlaneFacingNormal = 0.0f;
	float initialPlaneDist2Centre = 10.0f;

	float winMaxX;
	float winMaxY;
	float winMinX;
	float winMinY;

	//------ Debug Section ------//
	void debugLog(float deltaTime);

};

