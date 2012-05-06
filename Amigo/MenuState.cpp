#include "Context.h"
#include <GLTools.h>
#include "MenuState.h"
#include "IntroState.h"
#include "Sprite.h"
#include "Helper.h"
#include "GameEngine.h"
#include "Font.h"
#include <string>

MenuState::MenuState() {}

MenuState::~MenuState()
{
	printf("\n::: MenuState Exit :::\n\n");
}

void MenuState::Init()
{
	// Initialize members
	rot = 100;

	finishedLoading = false;
	printf("\n::: Menustate init :::\n\n");
}

bool MenuState::Load()
{
	if (rot == 100)
	{
		// Load sprites
		if (!sprBackground.LoadImage("res\\tx\\back\\back_1.png")) return false;
		if (!sprCursor.LoadImage("res\\tx\\ui\\cursor.png")) return false;
		sprCursor.setOrigin(3, 1);
		if (!sprTest.LoadImage("res\\hamster.png")) return false;
		sprTest.setOriginCenter();
		if (!sprUI.LoadImage("res\\tx\\ui\\ui.png")) return false;
		sprUI.setInterpolationMode(GL_NEAREST);

		// Load fonts
		FontRegular.LoadFont("res\\font\\robotor.ttf", 16);
		FontBold.LoadFont("res\\font\\robotob.ttf", 24);
	}

	// we're pretending that loading takes a lot of time
	rot--;
	if (rot > 0)
	{
		return false;
	}

	return true;
}

void MenuState::Update(GLdouble time)
{
	// Rotation effect-variable
	rot += time * 0.01f;
	if (rot >= 360)
		rot -= 360;

	// Get mouse position
	POINT mouse = Context::getMousePos();
}

void MenuState::Draw()
{
	// Background
	sprBackground.Draw(0, 0);

	// Flying hamsters
	sprTest.Draw(Context::getWindowWidth() / 2 + lenDirX(200, rot * 5), 200, lenDirX(20, rot * 10), 1.0f, 1.0f, 1.0f);

	// Draw some text
	std::string str;
	str = "Look at the flying hamster!";
	int charX, charW;
	float scale;
	charX = 0;
	for(int i = 0; i < str.length(); i++)
		scale = 1.0f + lenDirX(0.2f, -rot * 7 + i * 10);
	charW = FontBold.GetWidth(str) * scale;

	for(int i = 0; i < str.length(); i++)
	{
		FontBold.Draw(
			Context::getWindowWidth() / 2 - charW / 2 + charX + 2,
			130 + lenDirY(15, -rot * 8 + i * 10) + 2,
			str.substr(i, 1),
			lenDirY(9, rot * 8 + i * 10 - 90),
			scale, scale,
			0.0f,
			0.0f,
			0.0f,
			1.0f
			);
		FontBold.Draw(
			Context::getWindowWidth() / 2 - charW / 2 + charX,
			130 + lenDirY(15, -rot * 8 + i * 10),
			str.substr(i, 1),
			lenDirY(9, rot * 8 + i * 10 - 90),
			scale, scale,
			0.5f - lenDirX(0.2f, -rot * 15 + i * 10),
			0.5f - lenDirX(0.2f, -rot * 15 + i * 10),
			1.0f,
			1.0f
			);
		charX += FontBold.GetWidth(str.substr(i, 1));
	}

	// Draw the whole spritefont
	int charY = 10, charH, maxWidth = Context::getWindowWidth() - 10;
	charX = 10;
	charW = FontBold.GetWidth("Æ") + 4;
	charH = FontBold.GetHeight("|") + 4;
	for(int i = 32; i <= 255; i++)
	{
		std::string str = toStringChar((wchar_t)i);
		FontBold.Draw(charX, charY, str, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
		charX += charW;
		if (charX > maxWidth - charW)
		{
			charX = 10;
			charY += charH;
		}
	}

	// Get cursor position
	POINT mouse = Context::getMousePos();

	int x, y, w, h;

	// Draw a box
	x = 400;
	y = 400;
	w = 300;
	h = 200;

	std::string strTitle = "Title";

	sprUI.Draw(x - 10, y - 10, 0.0f, 1.0f, 1.0f, 1.0f, 39, 0, 10, 10); // Top left
	sprUI.Draw(x, y - 10, 0.0f, w, 1.0f, 1.0f, 49, 0, 1, 10); // Top
	sprUI.Draw(x + w, y - 10, 0.0f, 1.0f, 1.0f, 1.0f, 50, 0, 10, 10); // Top right
	sprUI.Draw(x + w, y, 0.0f, 1.0f, h, 1.0f, 50, 10, 10, 1); // Right
	sprUI.Draw(x + w, y + h, 0.0f, 1.0f, 1.0f, 1.0f, 50, 11, 10, 10); // Bottom right
	sprUI.Draw(x, y + h, 0.0f, w, 1.0f, 1.0f, 49, 11, 1, 10); // Bottom
	sprUI.Draw(x - 10, y + h, 0.0f, 1.0f, 1.0f, 1.0f, 39, 11, 10, 10); // Bottom left
	sprUI.Draw(x - 10, y, 0.0f, 1.0f, h, 1.0f, 39, 10, 10, 1); // Left
	sprUI.Draw(x, y, 0.0f, w, h, 0.5f, 49, 10, 1, 1); // Middle

	// Draw title over box
	if (strTitle != "")
	{
		sprUI.Draw(x, y - 40, 0.0f, 1.0f, 1.0f, 1.0f, 28, 0, 5, 36); // Left
		sprUI.Draw(x + 5, y - 40, 0.0f, w - 10, 1.0f, 1.0f, 33, 0, 1, 36); // Middle
		sprUI.Draw(x + w - 5, y - 40, 0.0f, 1.0f, 1.0f, 1.0f, 34, 0, 5, 36); // Right
		FontBold.Draw(x + 10, y - 31 - 1, strTitle, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.25f);
		FontBold.Draw(x + 10, y - 31, strTitle, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
	}

	// Draw some text in the box
	x += 10;
	y += 10;
	FontRegular.DrawFormatted(x, y, "This string is too damn long! Linebreak is tricky... Yes, it is highly difficult for me.", w - 20, 18);

	// Draw a button
	x += 10;
	y += -10 + h - 45;
	w = 200;
	h = 35;

	int hh = 17, xOff = 0;

	if (isInside(mouse.x, mouse.y, x, y, x + w, y + h))
	{
		xOff = 7;
		if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT))
		{
			xOff += 7;
		}
	}

	if (h > 35)
	{
		sprUI.Draw(x + w - 3, y + 17, 0.0f, 1.0f, h - 35, 1.0f, 4 + xOff, 17, 3, 1); // Right
		sprUI.Draw(x, y + 17, 0.0f, 1.0f, h - 35, 1.0f, xOff, 17, 3, 1); // Left
		sprUI.Draw(x + 3, y + 17, 0.0f, w - 6, h - 35, 1.0f, 3 + xOff, 17, 1, 1); // Middle
	}
	else
	{
		hh = h / 2;
	}
	sprUI.Draw(x, y, 0.0f, 1.0f, 1.0f, 1.0f, xOff, 0, 3, hh); // Top left
	sprUI.Draw(x + 3, y, 0.0f, w - 6, 1.0f, 1.0f, 3 + xOff, 0, 1, hh); // Top
	sprUI.Draw(x + w - 3, y, 0.0f, 1.0f, 1.0f, 1.0f, 4 + xOff, 0, 3, hh); // Top right
	sprUI.Draw(x + w - 3, y + h - 35 + (-hh + 34), 0.0f, 1.0f, 1.0f, 1.0f, 4 + xOff, 18 - hh + 16, 3, hh + 1); // Bottom right
	sprUI.Draw(x + 3, y + h - 35 + (-hh + 34), 0.0f, w - 6, 1.0f, 1.0f, 3 + xOff, 18 - hh + 16, 1, hh + 1); // Bottom
	sprUI.Draw(x, y + h - 35 + (-hh + 34), 0.0f, 1.0f, 1.0f, 1.0f, xOff, 18 - hh + 16, 3, hh + 1); // Bottom left

	std::string bStr = "Button";
	FontBold.Draw(x + (w - FontBold.GetWidth(bStr)) / 2, y + h / 2 - 11 - 1, bStr, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.25f);
	FontBold.Draw(x + (w - FontBold.GetWidth(bStr)) / 2, y + h / 2 - 11, bStr, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

	// Glow around button
	float glowAlpha = 0.0f;
	if (xOff == 7 || xOff == 14)
		glowAlpha = 0.35f + lenDirX(0.1f, rot * 50);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE); // Additive blending
	sprUI.Draw(x - 12, y - 12, 0.0f, 1.0f, 1.0f, glowAlpha, 0, 35, 15, 15); // Top left
	sprUI.Draw(x + 3, y - 12, 0.0f, w - 6, 1.0f, glowAlpha, 15, 35, 1, 15); // Top
	sprUI.Draw(x + w - 3 + 15, y - 12, 90.0f, 1.0f, 1.0f, glowAlpha, 0, 35, 15, 15); // Top right
	sprUI.Draw(x + w - 3 + 15, y + 3, 90.0f, h - 6, 1.0f, glowAlpha, 15, 35, 1, 15); // Right
	sprUI.Draw(x + w - 3 + 15, y + h + 12, 180.0f, 1.0f, 1.0f, glowAlpha, 0, 35, 15, 15); // Bottom right
	sprUI.Draw(x + w - 3, y + h + 12, 180.0f, w - 6, 1.0f, glowAlpha, 15, 35, 1, 15); // Bottom
	sprUI.Draw(x - 12, y + h + 12, 270.0f, 1.0f, 1.0f, glowAlpha, 0, 35, 15, 15); // Bottom left
	sprUI.Draw(x - 12, y + h - 3, 270.0f, h - 6, 1.0f, glowAlpha, 15, 35, 1, 15); // Left
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Reset to normal blending

	// Draw the cursor
	sprCursor.Draw(mouse.x, mouse.y);
}