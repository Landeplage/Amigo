#include "TestState.h"
#include "Input.h"
#include <GL\glew.h>

TestState::TestState() { }

TestState::~TestState()
{
	printf("\n::: TestState Exit :::\n\n");

	// Delete the rendertargets
	renderTargetTest->~RenderTarget();
}

void TestState::Init()
{
	printf("\n::: TestState Init :::\n\n");

	// Testing blending
	blend1 = 0;
	blend2 = 0;
}

bool TestState::Load()
{
	printf("TestState Load...\n");

	// Load background image
	if (!sprBackground.LoadImage("res//tx//back//back_2.png")) return false;

	// Load test-image
	if (!sprTest.LoadImage("res//test.png")) return false;

	// Load cursor image
	if (!sprCursor.LoadImage("res//gradient.png")) return false;

	// Create the rendertarget
	renderTargetTest = new RenderTarget(sprTest.getWidth(), sprTest.getHeight());

	return true;
}

void TestState::Update(GLdouble time)
{
	// An array of all possible blend-func parameters
	GLint ba[15];
	ba[0] = GL_ZERO;
	ba[1] = GL_ONE;
	ba[2] = GL_SRC_COLOR;
	ba[3] = GL_ONE_MINUS_SRC_COLOR;
	ba[4] = GL_DST_COLOR;
	ba[5] = GL_ONE_MINUS_DST_COLOR;
	ba[6] = GL_SRC_ALPHA;
	ba[7] = GL_ONE_MINUS_SRC_ALPHA;
	ba[8] = GL_DST_ALPHA;
	ba[9] = GL_ONE_MINUS_DST_ALPHA;
	ba[10] = GL_CONSTANT_COLOR;
	ba[11] = GL_ONE_MINUS_CONSTANT_COLOR;
	ba[12] = GL_CONSTANT_ALPHA;
	ba[13] = GL_ONE_MINUS_CONSTANT_ALPHA;
	ba[14] = GL_SRC_ALPHA_SATURATE;

	// Increment through the blend-modes
	if (Input::getMouseLeftPressed())
	{
		blend1 ++;
		if (blend1 > 14)
		{
			blend2 ++;
			if (blend2 > 14)
				blend2 = 0;
			blend1 = 0;
		}

		printf("Switched to blend modes: %i x %i\n", blend1, blend2);
	}

	if (Input::getMouseRightPressed())
	{
		blend1 --;
		if (blend1 < 0)
		{
			blend2 --;
			if (blend2 < 0)
				blend2 = 14;
			blend1 = 14;
		}

		printf("Switched to blend modes: %i x %i\n", blend1, blend2);
	}

	// Enable the rendertarget
	renderTargetTest->Begin();

	// Render test-sprite
	sprTest.Draw(0, 0);

	// Render cursor
	sprCursor.Draw(Input::getMousePos().x - sprCursor.getWidth() / 2, Input::getMousePos().y - sprCursor.getHeight() / 2);

	// Disable the rendertarget
	renderTargetTest->End();
}

void TestState::Draw()
{
	// Draw a background
	sprBackground.Draw(0, 0);

	// Draw test-sprite (directly to the default framebuffer)
	sprTest.Draw(0, 0);

	// Set blend mode
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	// Draw the rendertarget
	renderTargetTest->Draw(780, 0);
	// Reset blend mode
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Draw the gradient on top
	sprCursor.Draw(Input::getMousePos().x - sprCursor.getWidth() / 2, Input::getMousePos().y - sprCursor.getHeight() / 2);
	
}