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

	//------ ImGUI Section ------//
	bool isMainWindowOpen = true;
	float inputColour[4] = {0.5f, 0.5f, 0.5f, 1.0f};
	bool isSphere = false;
	bool isBox = false;
	bool isPlane = false;

	unsigned int flagsNumber = 3;
	unsigned int falgValue = 0;


	void ImGuiOptionCheck();

	//------ Debug Section ------//
	void debugLog(float deltaTime);

};

