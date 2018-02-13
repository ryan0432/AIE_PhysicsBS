#include "PhysicsEngineApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "PhysicsScene.h"
#include "Sphere.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <Gizmos.h>
#include <vector>
#include <iterator>



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

	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);

	m_physicsScene = new PhysicsScene();
	m_physicsScene->setTimeStep(0.01f);
	m_physicsScene->setGravity(glm::vec2(0, -2.0f));

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
	Sphere* rocketBall01;
	rocketBall01 = new Sphere(glm::vec2(50,-20),glm::vec2(0,0),50 , 10, glm::vec4(0,0,1,1));
	m_physicsScene->addActor(rocketBall01);
	//--------------------------------//


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
	//clear Gizmos
	aie::Gizmos::clear();

	//update physics scene
	m_physicsScene->update(deltaTime);
	m_physicsScene->updateGizmos();

	// input example
	aie::Input* input = aie::Input::getInstance();

	static const glm::vec4 colour[] =
	{
		glm::vec4(1,0,0,1), glm::vec4(0,1,0,1),
		glm::vec4(0,0,1,1), glm::vec4(0.8f,0,0.5f,1),
		glm::vec4(0,1,1,1)
	};

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
	{
		quit();
	}

	//------ Rocket Launch Test ------//

	Rigidbody* rocket = (Rigidbody*) (m_physicsScene->getActors()[0]);
	
	float keyTimer = 0;

	if (input->isKeyDown(aie::INPUT_KEY_UP))
	{
		float rocketMass = rocket->getMass();
		float gasMass = 1;
		keyTimer += deltaTime;

		if (rocketMass >= gasMass)
		{
			rocket->addForce(glm::vec2(0, gasMass * deltaTime * 0.01));
			rocket->setMass(rocketMass -= gasMass * deltaTime * 0.01);

			if ((int)keyTimer % 1 == 0)
			{
				Sphere* gas = new Sphere(rocket->getPosition(), glm::vec2(0, 0), gasMass, gasMass , glm::vec4(1, 0.5f, 0, 1));
				m_physicsScene->addActor(gas);
			}
		}
	}


	std::vector <Rigidbody*> deletingActorList;

	for (int i = 1; i < m_physicsScene->getActors().size(); )
	{
		if (((Rigidbody*)(m_physicsScene->getActors()[i]))->getPosition().y < -50)
		{
			deletingActorList.push_back( (Rigidbody*) (m_physicsScene->getActors()[i]) );
			//m_physicsScene->removeActor( (m_physicsScene->getActors()[i]) );
		}
		else
		{
			i++;
		}
	}

	for (auto pActor : deletingActorList)
	{
		if (deletingActorList.size() == 0)
			continue;
		delete pActor;
	}

	deletingActorList.clear();

	//make sure delete memory (change class remove func)
	//separate visual and actual force adding to the rocket
	//emit visual by second
	//store the search result in a new dynamicArray so we don't iter through the changing array (may cause problem)

	//--------------------------------//
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