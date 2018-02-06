#pragma once

#include "Application.h"
#include "Renderer2D.h"

class _2DProject2ndYearApp : public aie::Application
{
public:

	_2DProject2ndYearApp();
	virtual ~_2DProject2ndYearApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	aie::Renderer2D*	m_2dRenderer;
	aie::Font*			m_font;
};