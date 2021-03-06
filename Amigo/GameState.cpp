#include "GameState.h"
#include "Input.h"
#include "GameEngine.h"
#include "MenuState.h"
#include "Helper.h"
#include <math.h>
#include "GLFW.h"
#include "ResourceManager.h"

GameState::GameState()
{
	sprBackground = new Sprite();
	sprGround = new Sprite();
	sprPlayer = new Sprite();
	sprTest = new Sprite();
	sprPoint = new Sprite();

	font = new Font();
}

GameState::~GameState()
{
	printf("\n::: GameState Exit :::\n\n");
}

void GameState::AddResources()
{
	printf("GameState Load...\n");

	ResourceManager *rM = ResourceManager::GetInstance();

	// Add sprites to be loaded
	rM->AddSprite(sprBackground, "res\\tx\\back\\back_1.png");
	rM->AddSprite(sprGround, "res\\tx//env\\ground_graphic.png");
	rM->AddSprite(sprPlayer, "res\\hamster.png");
	rM->AddSprite(sprTest, "res\\tile2.png");
	rM->AddSprite(sprPoint, "res\\ellipse.png");

	// Add fonts to be loaded
	rM->AddFont(font, "res\\font\\robotob.ttf", 16);
}

void GameState::Init()
{
	printf("\n::: GameState Init...");

	// Create player
	player = Player();
	// Assign sprite to player
	player.SetSprite(sprPlayer);

	// Init curve-variables
	t = 0.0f;

	// Make a little test-curve
	AddVector(Vec2(400, 300));
	AddVector(Vec2(900, 170));
	AddVector(Vec2(750, 570));

	// Init debug variable
	mouseIsInsideSpline = false;

	printf("Done:::\n\n");
}

void GameState::HandleInput()
{
	// handle input
}

void GameState::Update(GLdouble time)
{
	// Exit to menu with ESC
	if (Input::GetKey(GLFW_KEY_ESCAPE))
	{
		GameEngine::GetInstance()->ChangeState(new MenuState());
	}

	// Update player
	player.Update(time);

	// Interpolation amount
	t += (float)(time * 0.0005);
	if (t >= splinePoints.size())
		t = 0.0f;

	// Add vectors to spline when mouse is pressed
	if (Input::getMouseLeftPressed())
	{
		last = AddVector(Input::getMousePos());
	}
	if (Input::getMouseLeft())
	{
		*last = Input::getMousePos() - splinePoints[splinePoints.size() - 1];
	}

	// Check if mouse is inside the spline
	mouseIsInsideSpline = isInsideSpline(Input::getMousePos());
}

void GameState::Draw()
{
	// Draw background
	sprBackground->Draw(Vec2(0, 0));

	// Draw ground
	sprGround->Draw(Vec2(0, 720 - 191));

	// Draw player
	player.Draw();

	// Draw points along the catmull-rom path
	if (splinePoints.size() > 1)
	{
		float dir, len;
		Vec2 h, hprev;
		h = GetCatmullrom(&splinePoints, &splineVelocities, 0.0f);
		hprev = h;

		for(float i = 0.0f; i < splinePoints.size(); i += 0.02f)
		{
			h = GetCatmullrom(&splinePoints, &splineVelocities, i);

			dir = Vec2Dir(hprev, h);
			len = Vec2Len(hprev, h);
			sprPoint->Draw(Vec2((int)(h.x - 1), (int)(h.y - 1)), 0.0f, Vec2(3, 3), 1.0f, 10, 10, 1, 1);

			hprev = h;
		}
	}

	// Draw spline-points and velocities
	if (splinePoints.size() > 0)
	{
		for(unsigned int i = 0; i < splinePoints.size(); i ++)
		{
			sprPoint->Draw(Vec2((int)splinePoints[i].x - 15, (int)splinePoints[i].y - 15));
			font->Draw(Vec2((int)splinePoints[i].x - 7, (int)splinePoints[i].y - 7), toString((int)i));
			sprPoint->Draw(Vec2((int)splinePoints[i].x, (int)splinePoints[i].y),
				Vec2Dir(splinePoints[i] + splineVelocities[i], splinePoints[i]),
				Vec2(Vec2Len(splinePoints[i] + splineVelocities[i], splinePoints[i]),
				1.0f), 0.5f, 10, 10, 1, 1);
		}
	}

	// Draw a point between every point in the spine
	/*if (splinePoints.size() > 1)
	{
		Vec2 h;
		float tTemp;
		tTemp = t;
		while(tTemp >= 1.0f)
			tTemp -= 1.0f;
		for(int n = 0; n < splinePoints.size(); n ++)
		{
			h = GetCatmullrom(&splinePoints, &splineVelocities, tTemp + n);
			sprPoint.Draw((int)(h.x - 15), (int)(h.y - 15));
		}
	}*/

	// Draw a single point that goes through all points of the spine
	if (splinePoints.size() > 1)
	{
		Vec2 h; 
		h = GetCatmullrom(&splinePoints, &splineVelocities, t);
		sprPoint->Draw(Vec2((int)(h.x - 15), (int)(h.y - 15)), 0.0f, Vec2(1.0f, 1.0f), Color(255, 128, 128), 1.0f);
	}

	// Draw a point the eases in/out
	float tTemp;
	tTemp = fmod(t, 1.0f) * 2.0f;
	if (tTemp > 1.0f)
		tTemp = 2.0f - tTemp;
	Vec2 v = Vec2(200 * EaseQuadInOut(tTemp), 100.0f);
	sprPoint->Draw(Vec2(20 + (int)(v.x - 15), (int)(v.y - 15)));

	// Draw a point at the mouse position
	Vec2 mouse = Input::getMousePos();
	sprPoint->Draw(Vec2(mouse.x, mouse.y), 0.0f, Vec2(1.0f, 1.0f), (float)mouseIsInsideSpline);

	// Debug text
	font->Draw(Vec2(10, 10), toString(t));
}

Vec2* GameState::AddVector(Vec2 position)
{
	splinePoints.push_back(position);
	splineVelocities.push_back(Vec2());
	return &splineVelocities[splinePoints.size() - 1];
}

bool GameState::isInsideSpline(Vec2 position)
{
	for(int i = 0; i < splinePoints.size(); i ++)
	{
		// do check? wut
	}

	return false;
}