#include "TestState.h"

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
}

bool TestState::Load()
{
	printf("TestState Load...\n");

	// Load background image
	if (!sprBackground.LoadImage("res//tx//back//back_2.png")) return false;

	// Load test-image
	if (!sprTest.LoadImage("res//test.png")) return false;

	// Create the rendertarget
	renderTargetTest = new RenderTarget(sprTest.getWidth(), sprTest.getHeight(), 1);

	return true;
}

void TestState::Update(GLdouble time)
{
	// Enable the rendertarget
	renderTargetTest->Begin();

	// Render test-sprite to rendertarget
	sprTest.Draw(0, 0);

	// Disable the rendertarget
	renderTargetTest->End();
}

void TestState::Draw()
{
	// Draw a background
	sprBackground.Draw(0, 0);

	// Draw test-sprite (directly to the default framebuffer)
	sprTest.Draw(0, 0);

	// Draw the rendertarget
	renderTargetTest->Draw(780, 0);
}