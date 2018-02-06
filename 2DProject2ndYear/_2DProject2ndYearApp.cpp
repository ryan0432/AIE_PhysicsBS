#include "_2DProject2ndYearApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <Gizmos.h>

_2DProject2ndYearApp::_2DProject2ndYearApp() {

}

_2DProject2ndYearApp::~_2DProject2ndYearApp() {

}

bool _2DProject2ndYearApp::startup() {
	
	aie::Gizmos::create(255U, 255U, 65535U, 65535U);

	m_2dRenderer = new aie::Renderer2D();

	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);
	
	return true;
}

void _2DProject2ndYearApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;
}

void _2DProject2ndYearApp::update(float deltaTime) {

	aie::Gizmos::clear();

	// input example
	aie::Input* input = aie::Input::getInstance();

	static float xPos = 0;
	xPos += 1.0f * deltaTime;
	aie::Gizmos::add2DCircle(glm::vec2(xPos,0), 10.0f, 32, glm::vec4(1,1,0,1));

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void _2DProject2ndYearApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	// draw your stuff here!
	
	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	float aspectRatio = (float)getWindowWidth() / (float)getWindowHeight();
	aie::Gizmos::draw2D( glm::ortho<float>( -100.0f, 100.0f,
										  -100.0f / aspectRatio, 100.0f / aspectRatio,
										  -1.0f, 1.0f) );

	// done drawing sprites
	m_2dRenderer->end();
}