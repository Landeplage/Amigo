#include "BlendtestState.h"
#include "ResourceManager.h"
#include "Input.h"
#include "ShaderManager.h"

BlendtestState::BlendtestState()
{
	rt = new RenderTarget(0, 0);
	rt2 = new RenderTarget(0, 0);

	sprTest = new Sprite();
	sprBg = new Sprite();
	font = new Font();
}

BlendtestState::~BlendtestState()
{
	// destroy
}

void BlendtestState::AddResources()
{
	ResourceManager *rm = ResourceManager::GetInstance();

	rm->AddSprite(sprTest, "res\\gradient.png");
	rm->AddSprite(sprBg, "res\\tx\\back\\back_2.png");
	rm->AddFont(font, "res\\font\\robotob.ttf", 12);
}

void BlendtestState::Init()
{
	// Resize rendertargets
	rt->SetSize(Vec2(sprTest->getWidth(), sprTest->getHeight()));
	rt2->SetSize(Vec2(sprTest->getWidth(), sprTest->getHeight()));

	// Render test-image to rendertarget 1
	rt->Begin();
	sprTest->Draw(Vec2(0, 0));
	rt->End();

	// Render rendertarget 1 to rendertarget 2
	rt2->Begin();
	sprTest->Draw(Vec2(0, 0));
	rt->Draw(50, 0);
	rt2->End();

	// Initialize shader
	ShaderManager::GetInstance();
}

void BlendtestState::HandleInput()
{
	// handle input
}

void BlendtestState::Update(GLdouble time)
{
	// update
}

void BlendtestState::Draw()
{
	// Set alpha
	float alpha = Input::getMousePos().x / 1280;

	// Draw background
	sprBg->Draw(Vec2(0, 0), 0.0f, Vec2(1280.0f, 1.0f), Color(255, 255, 255), 1.0f, 0, 0, 1, 720);

	// Draw test-image
	sprTest->Draw(Vec2(200, 200), 0.0f, Vec2(1.0f, 1.0f), alpha);
	//sprTest->Draw(250, 200, 0.0f, 1.0f, 1.0f, alpha);

	// Draw rendertargets
	ShaderManager::GetInstance()->Apply();
	rt->Draw(500, 200, alpha);
	//rt2->Draw(800, 200, alpha);
	ShaderManager::GetInstance()->Reset();

	// Draw some text
	font->Draw(Vec2(10, 10), "Hello, this is text.", 0.0f, Vec2(1.0f, 1.0f), Color(255, 255, 255), alpha);
}