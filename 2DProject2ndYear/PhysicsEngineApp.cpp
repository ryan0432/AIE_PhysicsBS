#include "PhysicsEngineApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "PhysicsScene.h"
#include "Sphere.h"
#include "Plane.h"
#include "Box.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <Imgui.h>

#include <Gizmos.h>
#include <vector>
#include <iterator>
#include <iostream>



PhysicsEngineApp::PhysicsEngineApp()
{
}


PhysicsEngineApp::~PhysicsEngineApp()
{
}

bool PhysicsEngineApp::startup()
{
	aie::Gizmos::create(255U, 255U, 65535U, 65535U);

	m_2dRenderer = new aie::Renderer2D();

	aspectRatio = (float)getWindowWidth() / (float)getWindowHeight();

	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);

	m_physicsScene = new PhysicsScene();
	m_physicsScene->setTimeStep(0.01f);
	m_physicsScene->setGravity(glm::vec2(0, -9.8));

#pragma region RocketTest
	//------ Newton's 2nd law test ------//
	//Sphere* ball01;
	//ball01 = new Sphere(glm::vec2(-40, 0), glm::vec2(0, 0), 1.0f, 5.0f, glm::vec4(0, 0, 1, 1));
	//m_physicsScene->addActor(ball01);

	//Sphere* ball02;
	//ball02 = new Sphere(glm::vec2(40, 0), glm::vec2(0, 0), 1.0f, 5.0f, glm::vec4(1, 0, 0, 1));
	//m_physicsScene->addActor(ball02);
	//
	//ball01->addForce(glm::vec2(30, 0));
	//ball02->addForce(glm::vec2(-10, 0));
	//-----------------------------------//

	//------ Rocket Launch Test ------//
	// To fix deltaTime issue, move Rocket object into a Rocket class which derives from Sphere class or Rigidbody class
	// So the fixedUpdate won't interfere the PhysicsScene
	// [DON'T directly call fixedUpdate from newed rocket object from App, it will break the PhysicsScene
	//Sphere* rocketBall01;
	//rocketBall01 = new Sphere(glm::vec2(0,0),glm::vec2(0,0), rocketTotallMass, rocketRadius, glm::vec4(0,0,1,1));
	//m_physicsScene->addActor(rocketBall01);
	//--------------------------------//

	//------ Collision Detection Test ------//
#pragma endregion

	Plane* plane01; //floor
	plane01 = new Plane(glm::vec2(0, 1), glm::vec4(1, 1, 1, 1), -49);
	m_physicsScene->addActor(plane01);

	Plane* plane02; //ceiling
	plane02 = new Plane(glm::vec2(0, -1), glm::vec4(1, 1, 1, 1), -49);
	m_physicsScene->addActor(plane02);

	Plane* plane03; //left wall
	plane03 = new Plane(glm::vec2(1, 0), glm::vec4(1, 1, 1, 1), -90);
	m_physicsScene->addActor(plane03);

	Plane* plane04; //right wall
	plane04 = new Plane(glm::vec2(-1, 0), glm::vec4(1, 1, 1, 1), -90);
	m_physicsScene->addActor(plane04);

	Sphere* ball01;
	ball01 = new Sphere(glm::vec2(10, 0), glm::vec2(-20, 0), 1.0f, 10.0f, glm::vec4(0, 0, 1, 1));
	m_physicsScene->addActor(ball01);

	Sphere* ball02;
	ball02 = new Sphere(glm::vec2(0, 30), glm::vec2(0, 0), 1.0f, 5.0f, glm::vec4(1, 0, 0, 1));
	m_physicsScene->addActor(ball02);
	
	Box* box01;
	box01 = new Box(glm::vec2(-40, 0), glm::vec2(20, 0), 2.0f, glm::vec2(3, 6), glm::vec4(1, 0, 0, 1));
	m_physicsScene->addActor(box01);
	
	Box* box02;
	box01 = new Box(glm::vec2(40, 0), glm::vec2(-20, 0), 2.0f, glm::vec2(3, 6), glm::vec4(1, 1, 0, 1));
	m_physicsScene->addActor(box01);

	return true;
}

void PhysicsEngineApp::shutdown()
{
	delete m_font;
	delete m_2dRenderer;
	delete m_physicsScene;
}

void PhysicsEngineApp::update(float deltaTime)
{
	//input reference
	aie::Input* input = aie::Input::getInstance();

	//clear Gizmos
	aie::Gizmos::clear();

	//------ ImGUI Section ------//
	ImGui::Begin("Physics Object Customizer", &isMainWindowOpen, ImVec2(300, 500), 0.5);
	ImGui::ColorEdit4("Colour", inputColour, true);
	ImGui::Text("Shape Type");

	ImGui::RadioButton("Sphere",&currShapeType, 0); ImGui::SameLine();
	ImGui::RadioButton("Box", &currShapeType, 1); ImGui::SameLine();
	ImGui::RadioButton("Plane", &currShapeType, 2);

	winPos = screenRatioConverter(ImGui::GetWindowPos().x, (float)getWindowHeight() - ImGui::GetWindowPos().y);
	winSize = glm::vec2(ImGui::GetWindowContentRegionMax().x * 100, ImGui::GetWindowContentRegionMax().y * (100.0f / aspectRatio));

	switch (currShapeType)
	{
		case 0:
		{
			ImGui::DragFloat("Sphere Radius", &initialSphereRadius, 0.1f, 1.0f, 50.0f);
			ImGui::SliderFloat2("Velocity (X, Y)", initialVelocity, -50.0f, 50.0f);
			ImGui::DragFloat("Mass", &initialMass, 1.0f, 0.1f, 20.0f);
			break;
		}

		case 1:
		{
			ImGui::DragFloat2("Box Size (W, H)", initialBoxDemention, 0.1f, 1.0f, 50.0f);
			ImGui::SliderFloat2("Velocity (X, Y)", initialVelocity, -50.0f, 50.0f);
			ImGui::DragFloat("Mass", &initialMass, 1.0f, 0.1f, 20.0f);
			break;
		}

		case 2:
		{
			ImGui::SliderAngle("Face Normal", &initialPlaneFacingNormal, 0.0f, 360.0f);
			ImGui::DragFloat("Dist to Centre", &initialPlaneDist2Centre, 0.1f, -50.0f, 50.0f);
			break;
		}

		default:
		{
			0;
			break;
		}
	}

	ImGui::End();
	ImGui::Render();

	//------ Create Objects By Input ------//
	if (input->wasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_LEFT))
	{
		glm::vec2 mousePos = screenRatioConverter((float)input->getMouseX(), (float)input->getMouseY());

		if(ImGui::IsMouseHoveringAnyWindow() == false)
		{
			switch (currShapeType)
			{
				case 0:
				{
					Sphere* sphere = new Sphere(mousePos, glm::vec2(initialVelocity[0], initialVelocity[1]),
						initialMass, initialSphereRadius,
						glm::vec4(inputColour[0], inputColour[1], inputColour[2], inputColour[3]));
					m_physicsScene->addActor(sphere);
					break;
				}

				case 1:
				{
					Box* box = new Box(mousePos, glm::vec2(initialVelocity[0], initialVelocity[1]),
						initialMass, glm::vec2(initialBoxDemention[0] / 2, initialBoxDemention[1] / 2),
						glm::vec4(inputColour[0], inputColour[1], inputColour[2], inputColour[3]));
					m_physicsScene->addActor(box);
					break;
				}

				case 2:
				{
					Plane* plane = new Plane(glm::vec2(glm::sin(initialPlaneFacingNormal), glm::cos(initialPlaneFacingNormal)),
						glm::vec4(inputColour[0], inputColour[1], inputColour[2], inputColour[3]),
						initialPlaneDist2Centre);
					m_physicsScene->addActor(plane);
					break;
				}

				default:
				{
					0;
					break;
				}
			}
		}
		else
		{
			return;
		}
	}

	static const glm::vec4 colour[] =
	{
		glm::vec4(1,0,0,1), glm::vec4(0,1,0,1),
		glm::vec4(0,0,1,1), glm::vec4(0.8f,0,0.5f,1),
		glm::vec4(0,1,1,1)
	};

	//------ update physics scene ------//
	m_physicsScene->update(deltaTime);
	m_physicsScene->updateGizmos();

	//------ exit the application ------//
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
	{
		quit();
	}

#pragma region RocketTest

	//------ Rocket Launch Test Start ------//
	//------ Part1: Create Emission Balls ------//
	//Rigidbody* rocket = (Rigidbody*) (m_physicsScene->getActors()[0]);

	//if (input->isKeyDown(aie::INPUT_KEY_UP))
	//{
	//	rocketTotallMass = rocket->getMass();

	//	if (rocketTotallMass >= rocketShellMass)
	//	{
	//		rocket->addForce(glm::vec2(0, gasMass * 500 * deltaTime)); //*
	//		rocket->setMass(rocketTotallMass -= gasMass * deltaTime);

	//		if (emissionTimer(deltaTime, 0.3f))
	//		{
	//			Sphere* gas = new Sphere(glm::vec2(rocket->getPosition().x, (rocket->getPosition().y - rocketRadius - gasRadius - 1)),
	//									 glm::vec2(0, -2.0f), gasMass, gasRadius, glm::vec4(1, 0.5f, 0, 1));
	//			m_physicsScene->addActor(gas);
	//		}
	//	}
	//}

	//------ Part2-method1: Delete Emission Balls ------//
	//std::vector <PhysicsObject*> deletingActorList;
	//int actorListSize = m_physicsScene->getActors().size();

	//for (int i = 1; i < actorListSize; ++i)
	//{
	//	if (actorListSize < 1)
	//		continue;

	//	if (((Rigidbody*)(m_physicsScene->getActors()[i]))->getPosition().y < -55)
	//	{
	//		deletingActorList.push_back(m_physicsScene->getActors()[i]);
	//	}
	//}

	//for (auto i : deletingActorList)
	//{
	//	for (auto j : m_physicsScene->getActors())
	//	{
	//		if (i == j)
	//		{
	//			m_physicsScene->removeActor(j);
	//		}
	//	}
	//}

	//deletingActorList.clear();

	//make sure delete memory (change class remove func)
	//separate visual and actual force adding to the rocket
	//emit visual by second
	//store the search result in a new dynamicArray so we don't iter through the changing array (may cause problem)
#pragma endregion

	//------ Debug Log (Console) ------//
	debugLog(deltaTime);
}

void PhysicsEngineApp::draw()
{
	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	// draw your stuff here!
	float aspectRatio = (float)getWindowWidth() / (float)getWindowHeight();

	aie::Gizmos::draw2D(glm::ortho<float>(-100.0f, 100.0f,
		-100.0f / aspectRatio, 100.0f / aspectRatio,
		-1.0f, 1.0f));

	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// done drawing sprites
	m_2dRenderer->end();
}

#pragma region RocketTestFunctions
//bool PhysicsEngineApp::emissionTimer(float deltaTime, float emissionRate)
//{
//	keyTimer += deltaTime;
//
//	if (keyTimer >= emissionRate)
//	{
//		keyTimer = 0;
//		return true;
//	}
//
//	return false;
//}
#pragma endregion

glm::vec2 PhysicsEngineApp::screenRatioConverter(float x, float y)
{
	float posX = ((x / (float)getWindowWidth()) * 2 - 1) * 100.0f;
	float posY = ((y / (float)getWindowHeight()) * 2 - 1 ) * (100.0f / aspectRatio);

	return glm::vec2(posX, posY);
}

void PhysicsEngineApp::debugLog(float deltaTime)
{
#pragma region RocketTest
	//std::cout << "Delta Time: " << deltaTime << std::endl;
	//std::cout << "Key Timer: " << keyTimer << std::endl;
	//std::cout << "Actor's Array Size: " << m_physicsScene->getActors().size() << std::endl;
	//std::cout << "Rocket Totall Mass: " << rocketTotallMass << std::endl;
	//std::cout << "Gas Mass: " << gasMass << std::endl;
	//m_physicsScene->debugScene();
#pragma endregion

	std::cout << "Win Pos: (" << winPos.x << ", " << winPos.y << ")" << std::endl;
	std::cout << "Win Siz: (" << winSize.x << ", " << winSize.y << ")" << std::endl;

	system("cls");
}