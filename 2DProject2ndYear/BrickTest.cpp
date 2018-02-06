#include "BrickTest.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "PhysicsScene.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <Gizmos.h>

BrickTest::BrickTest()
{
	
}


BrickTest::~BrickTest()
{
}

bool BrickTest::startup()
{
	aie::Gizmos::create(255U, 255U, 65535U, 65535U);

	m_2dRenderer = new aie::Renderer2D();

	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);

	m_physicsScene = new PhysicsScene();
	m_physicsScene->setTimeStep(0.01f);

	return true;
}

void BrickTest::shutdown()
{
	delete m_font;
	delete m_2dRenderer;
	delete m_physicsScene;
}

void BrickTest::update(float deltaTime)
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

	static const int rows = 5;
	static const int columns = 10;
	static const int hSpace = 1;
	static const int vSpace = 1;

	static const glm::vec2 screenSize(100, 50);
	static const glm::vec2 brickSize(7, 3);
	static const glm::vec2 startPos( -(columns >> 1) * ( (brickSize.x * 2) + vSpace) + brickSize.x + (vSpace / 2.0f), screenSize.y - ((brickSize.y * 2) + hSpace) );
	
	// draw the grid of blocks
	glm::vec2 brickPos;

	for (int y = 0; y < rows; y++)
	{
		brickPos = glm::vec2(startPos.x, startPos.y - (y* ((brickSize.y * 2) + hSpace)));

		for (int x = 0; x < columns; x++)
		{
			aie::Gizmos::add2DAABBFilled(brickPos, brickSize, colour[y]); brickPos.x += (brickSize.x * 2) + vSpace;
		}
	}

	// draw the ball
	aie::Gizmos::add2DCircle(glm::vec2(0, -35), 3, 12, glm::vec4(1, 1, 0, 1));

	// draw the player’s paddle
	aie::Gizmos::add2DAABBFilled(glm::vec2(0, -40), glm::vec2(12, 2),
								 glm::vec4(1, 0, 1, 1));

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
	{
		quit();
	}

}

void BrickTest::draw()
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