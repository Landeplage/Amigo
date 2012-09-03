#include "MenuSystem.h"
#include "Helper.h"
#include <math.h>
#include "Input.h"
#include "Context.h"

MenuSystem::MenuSystem(Sprite* sprCursor, Sprite* sprUI, Font* fontBold, Font* fontRegular)
{
	printf("-> MenuSystem created.\n");

	// Initialize variables
	ResetFocus();
	this->sprUI = sprUI;
	this->fontBold = fontBold;
	this->fontRegular = fontRegular;
	this->sprCursor = sprCursor;
	rot = 0;
	cursorOffset = 0;
	overlayShow = false;
	overlaySlide = -1;
	overlaySlideTarget = -1;
	overlaySlideSpeed = 0;
	overlayRenderTarget = new RenderTarget(420, Context::getWindowHeight());
	menuRenderTarget = new RenderTarget(Context::getWindowWidth(), Context::getWindowHeight());
	onButton1 = [](){};
	onButton2 = [](){};

	// Tooltip variables
	tooltipPosition = Vec2(0, 0);
	tooltipSize = Vec2(0, 0);
	tooltipString = "?";
	tooltipTimer = -1.0;

	// Add items to overlay
	overlayItems.push_back(new Box(this, "", 10, 10, 10, 10, 0));
	overlayBox = (Box*)overlayItems[overlayItems.size() - 1];
	overlayItems.push_back(new Button(this, "", 0, 0, 0, 0, MenuItem::CENTER, 0, "", [](){}));
	overlayButton1 = (Button*)overlayItems[overlayItems.size() - 1];
	overlayItems.push_back(new Button(this, "", 0, 0, 0, 0, MenuItem::CENTER, 0, "", [](){}));
	overlayButton2 = (Button*)overlayItems[overlayItems.size() - 1];

	blend1 = 0;
	blend2 = 0;
}

MenuSystem::~MenuSystem()
{
	printf("Deleting MenuSystem...\n");

	// Delete all menus
	for(GLuint i = 0; i < menus.size(); i ++)
	{
		menus[i]->~Menu();
	}

	// Delete rendertargets
	overlayRenderTarget->~RenderTarget();
	menuRenderTarget->~RenderTarget();

	// Delete overlay-items
	for(GLuint i = 0; i < overlayItems.size(); i ++)
	{
		overlayItems[i]->Unload();
	}
}

void MenuSystem::Update(GLdouble time)
{
	// Pulsing variable for various effects
	rot += (GLfloat)time * 0.01f;
	if (rot >= 360)
		rot -= 360;

	// Reset cursor offset
	//cursorOffset = 0;

	// Update all menus
	for(GLuint i = 0; i < menus.size(); i ++)
	{
		menus[i]->Update(time);
	}

	// Handle overlay-menus
	if (abs(overlaySlide) < 0.1f && overlayShow)
	{
		// Update overlay items
		for(GLuint i = 0; i < overlayItems.size(); i ++)
		{
			overlayItems[i]->Update(time);
		}
	}

	// Slide-variable for the overlay
	overlaySlideSpeed = Wobble(overlaySlide, overlaySlideTarget, overlaySlideSpeed, 0.1f, 0.1f, time);
	overlaySlide += overlaySlideSpeed * (GLfloat)time;
	if (!overlayShow && overlaySlide > 1)
	{
		overlaySlideSpeed = 0;
		overlaySlide = -1;
		overlaySlideTarget = -1;
	}
	else
	{
		if (overlayShow && overlaySlideSpeed < 0 && overlaySlide < 0)
		{
			overlaySlide = 0;
			overlaySlideSpeed = 0;
		}
	}

	// Tooltip-timer
	if (tooltipTimer > 0)
	{
		tooltipTimer -= time / 1000;
		if (tooltipTimer <= 0)
			tooltipTimer = 0;
	}
}

void MenuSystem::Draw()
{
	// Draw debug-text
	Vec2 testVec;
	testVec = Vec2(2, 2) + Vec2(2, 2);
	std::string str = "Vec2(2, 2) + Vec2(2, 2) = [" + toString(testVec.x) + "][" + toString(testVec.y) + "]";
	fontBold->Draw((Context::getWindowWidth() - fontBold->GetWidth(str)) / 2, 10, str);

	// Begin menu-rendertarget
	//menuRenderTarget->Begin();

	// Draw menus
	for(GLuint i = 0; i < menus.size(); i ++)
	{
		menus[i]->Draw();
	}

	/*
	// End menu-rendertarget
	menuRenderTarget->End();

	// Set blend mode
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	// Draw menu-rendertarget
	GLfloat mod = (abs(overlaySlide) * 0.5f) + 0.5f;
	menuRenderTarget->Draw(0, 0, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f // BUG: rendertarget blends incorrectly when drawn with alpha less than 1.0 (try entering "mod" here)
		, 0, 0, (GLint)menuRenderTarget->GetSize().x, (GLint)menuRenderTarget->GetSize().y);
	// Reset blend mode
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	*/

	// Draw overlay for messages/questions
	if (abs(overlaySlide + 1) > 0)
	{
		// Black transparent fullscreen box
		sprUI->Draw(0, 0, 0.0f, (GLfloat)Context::getWindowWidth(), (GLfloat)Context::getWindowHeight(), 0.0f, 0.0f, 0.0f, 0.5f * (1 - abs(overlaySlide)), 49, 10, 1, 1);
		
		// Set blend mode
		//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

		// Rendertarget begin
		//overlayRenderTarget->Begin();

		// Items of the overlay
		for(GLint i = 0; i < (GLint)overlayItems.size(); i ++)
		{
			overlayItems[i]->Draw(abs(overlaySlide));
		}

		// Message-text
		fontRegular->DrawLinebreak((GLint)overlayBox->GetPosition().x + 10, (GLint)overlayBox->GetPosition().y + 10, overlayText, (GLint)overlayBox->GetSize().x - 20, 18);

		// Rendertarget end
		//overlayRenderTarget->End();

		// Set blend mode
		//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	
		// Draw rendertarget
		/*GLfloat overlayScale = (abs(overlaySlide) + 1) * 0.2f + 0.8f;
		overlayRenderTarget->Draw(
			(GLint)((Context::getWindowWidth() - overlayRenderTarget->GetSize().x * overlayScale) / 2),
			(GLint)(-(overlayRenderTarget->GetSize().y * (overlayScale - 1)) / 2),
			0.0f,
			overlayScale,
			overlayScale,
			1.0f,
			1.0f,
			1.0f,
			1 - abs(overlaySlide),
			0,
			0,
			(GLint)overlayRenderTarget->GetSize().x,
			(GLint)overlayRenderTarget->GetSize().y);

		// Reset blend mode
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		*/
	}

	// Draw tooltip
	if (tooltipTimer == 0)
		DrawTooltip();

	// Draw cursor
	if (cursorOffset > -1)
	{
		Vec2 mouse = Input::getMousePos();
		sprCursor->Draw((GLint)mouse.x, (GLint)mouse.y, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, cursorOffset * 32, 0, 32, 32);
	}
}

// Add a menu
Menu* MenuSystem::AddMenu(GLint x, GLint y, GLint width, GLint height)
{
	menus.push_back(new Menu(x, y, width, height, this));
	return menus[menus.size() - 1];
}

// Make a variable wobble to target value
GLfloat MenuSystem::Wobble(GLfloat currentX, GLfloat targetX, GLfloat currentSpeed, GLfloat force, GLfloat friction, GLdouble time)
{
	if (currentX < targetX) { currentSpeed += (targetX - currentX) * force * 0.01f; }
	else if (currentX > targetX) { currentSpeed -= (currentX - targetX) * force * 0.01f; }
    currentSpeed = currentSpeed * (1.0f - friction);
    return currentSpeed;
}

// Set focus
bool MenuSystem::SetFocus(MenuItem* menuItem)
{
	if (focusedItem == NULL)
	{
		focusedItem = menuItem;
		return true;
	}
	return false;
}

// Reset focus
void MenuSystem::ResetFocus()
{
	focusedItem = NULL;
}

// Get focus
MenuItem* MenuSystem::GetFocus()
{
	return focusedItem;
}

// Return the value of rot
GLfloat MenuSystem::GetRot()
{
	return rot;
}

// Return the bold font
Font* MenuSystem::GetFontBold()
{
	return fontBold;
}

// Return the regular font
Font* MenuSystem::GetFontRegular()
{
	return fontRegular;
}

// Return the UI-sprite
Sprite* MenuSystem::GetSpriteUI()
{
	return sprUI;
}

// Get the overlay-slide transition
GLfloat MenuSystem::GetOverlaySlide()
{
	return overlaySlide;
}

// Get the overlay show-variable
bool MenuSystem::GetOverlayShow()
{
	return overlayShow;
}

// Set cursor offset. -1 means invisible cursor
void MenuSystem::SetCursor(GLint cursorOffset)
{
	this->cursorOffset = cursorOffset;
}

// Show a message-overlay on screen
void MenuSystem::ShowMessage(std::string title, std::string text)
{
	ShowMessage(title, text, [](){});
}

// Show a message-overlay on screen
void MenuSystem::ShowMessage(std::string title, std::string text, std::function<void()> onButton1)
{
	OverlayInit(title, text);

	// Setup "Okay"-button
	overlayButton1->SetSize(200, 35);
	overlayButton1->SetText("Okay", MenuItem::CENTER);
	overlayButton1->SetPosition((GLint)overlayBox->GetPosition().x + (GLint)overlayBox->GetSize().x / 2 - 100, (GLint)overlayBox->GetPosition().y + (GLint)overlayBox->GetSize().y + 15);
	//overlayButton1->SetMenuOffset((Context::getWindowWidth() - (GLint)overlayRenderTarget->GetSize().x) / 2, 0);
	overlayButton1->SetOnClick([=](){ this->OnButton1(); });
	this->onButton1 = onButton1;

	// Hide the other button
	overlayButton2->active = false;
	overlayButton2->visible = false;
}

// Show a question on screen, with two options
void MenuSystem::ShowQuestion(std::string title, std::string text, std::function<void()> onButton1, std::function<void()> onButton2)
{
	OverlayInit(title, text);

	// Setup "Yes"-button
	overlayButton1->SetSize(195, 35);
	overlayButton1->SetText("Yes", MenuItem::CENTER);
	overlayButton1->SetPosition((GLint)overlayBox->GetPosition().x, (GLint)(overlayBox->GetPosition().y + overlayBox->GetSize().y + 15));
	//overlayButton1->SetMenuOffset((GLint)((Context::getWindowWidth() - overlayRenderTarget->GetSize().x) / 2), 0);
	overlayButton1->SetOnClick([=](){ this->OnButton1(); });
	this->onButton1 = onButton1;

	// Setup "No" button
	overlayButton2->SetSize(195, 35);
	overlayButton2->SetText("No", MenuItem::CENTER);
	overlayButton2->SetPosition((GLint)(overlayBox->GetPosition().x + overlayBox->GetSize().x - overlayButton2->GetSize().x), (GLint)(overlayBox->GetPosition().y + overlayBox->GetSize().y + 15));
	//overlayButton2->SetMenuOffset((GLint)((Context::getWindowWidth() - overlayRenderTarget->GetSize().x) / 2), 0);
	overlayButton2->SetOnClick([=](){ this->OnButton2(); });
	this->onButton2 = onButton2;

	// Make sure the "No" button is visible and active
	overlayButton2->active = true;
	overlayButton2->visible = true;
}

// Initiate the overlay-box
void MenuSystem::OverlayInit(std::string title, std::string text)
{
	// Setup overlay
	overlayText = text;
	overlayShow = true;
	overlaySlideTarget = 0;
	ResetFocus();

	// Setup box
	overlayBox->SetTitle(title);
	overlayBox->SetSize(400, fontRegular->GetHeight(overlayText, 400 - 20, 18) + 20);
	overlayBox->SetPosition((GLint)(Context::getWindowWidth() / 2 - overlayBox->GetSize().x / 2), (GLint)((Context::getWindowHeight() - overlayBox->GetSize().y) / 2));
}

// Hide the overlay
void MenuSystem::HideOverlay()
{
	overlayShow = false;
	overlaySlideTarget = 1.2f;
	ResetFocus();
}

// Perform the action of Button 1
void MenuSystem::OnButton1()
{
	onButton1();
	HideOverlay();
}

// Perform the action of Button 2
void MenuSystem::OnButton2()
{
	onButton2();
	HideOverlay();
}

// Initiate the tooltip
void MenuSystem::SetTooltip(Vec2 position, std::string tooltipString)
{
	this->tooltipString = tooltipString;
	tooltipPosition = position;

	// Calculate size of tooltip-box
	tooltipSize = Vec2((GLfloat)250, (GLfloat)(fontRegular->GetHeight(tooltipString, 250, 17) + 16));
	if (fontRegular->GetWidth(tooltipString) < 250)
		tooltipSize.x = (GLfloat)(fontRegular->GetWidth(tooltipString) + 16);

	tooltipTimer = 1.2;
}

// Hide the tooltip
void MenuSystem::ResetTooltip()
{
	tooltipTimer = -1;
}

// Draw the tooltip
void MenuSystem::DrawTooltip()
{
	GLint x, y, w, h;
	x = (GLint)tooltipPosition.x;
	y = (GLint)tooltipPosition.y;
	w = (GLint)tooltipSize.x;
	h = (GLint)tooltipSize.y;

	// Line it up properly
	x -= (GLint)tooltipSize.x / 2;
	y -= h + 11;

	// Make sure box stays inside screen boundaries
	if (x > Context::getWindowWidth() - (GLint)(tooltipSize.x))
		x = Context::getWindowWidth() - (GLint)(tooltipSize.x);
	if (x < 0)
		x = 0;
	if (y > Context::getWindowHeight() - (GLint)(tooltipSize.y))
		y = Context::getWindowHeight() - (GLint)(tooltipSize.y);
	if (y < 0)
		y = 0;

	// Pointer position (relative)
	GLint pointerX;
	pointerX = (GLint)tooltipPosition.x - x - 11;
	if (pointerX < 4)
		pointerX = 4;
	if (pointerX > w - 26)
		pointerX = w - 26;

	// Transparency
	GLfloat alpha;
	alpha = 1.0f;

	// Tooltip-box
	sprUI->Draw(x - 1, y, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, alpha, 25, 45, 5, 4); // Left top corner
	sprUI->Draw(x - 1, y + 4, 0.0f, 1.0f, (GLfloat)(h - 8) / 20.0f, 1.0f, 1.0f, 1.0f, alpha, 25, 49, 5, 20); // Left side
	sprUI->Draw(x - 1, y + h - 4, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, alpha, 25, 69, 5, 7); // Left bottom corner
	sprUI->Draw(x + w - 4, y + h - 4, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, alpha, 31, 69, 5, 7); // Right bottom corner
	sprUI->Draw(x + w - 4, y + 4, 0.0f, 1.0f, (GLfloat)(h - 8) / 20.0f, 1.0f, 1.0f, 1.0f, alpha, 31, 49, 5, 20); // Left side
	sprUI->Draw(x + w - 4, y, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, alpha, 31, 45, 5, 4); // Right top corner
	sprUI->Draw(x + 4, y, 0.0f, (GLfloat)(w - 8), 1.0f, 1.0f, 1.0f, 1.0f, alpha, 30, 45, 1, 4); // Top
	sprUI->Draw(x + 4, y + 4, 0.0f, (GLfloat)(w - 8), (GLfloat)(h - 8) / 20.0f, 1.0f, 1.0f, 1.0f, alpha, 30, 49, 1, 20); // Mid
	sprUI->Draw(x + 4, y + h - 4, 0.0f, (GLfloat)(pointerX - 4), 1.0f, 1.0f, 1.0f, 1.0f, alpha, 30, 69, 1, 7); // Bottom Left
	sprUI->Draw(x + pointerX + 22, y + h - 4, 0.0f, (GLfloat)(w - pointerX - 22 - 4), 1.0f, 1.0f, 1.0f, 1.0f, alpha, 30, 69, 1, 7); // Bottom Right
	sprUI->Draw(x + pointerX, y + h - 4, 0.0f, 22, 1.0f, 1.0f, 1.0f, 1.0f, alpha, 30, 69, 1, 3); // Patch above pointer
	sprUI->Draw(x + pointerX, y + h - 1, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, alpha, 18, 31, 22, 13); // Pointer

	// Tooltip text
	fontRegular->DrawLinebreak(x + 8, y + 8, tooltipString, w - 10, 17, 1.0f, 1.0f, 1.0f, alpha);
	fontRegular->DrawLinebreak(x + 8, y + 7, tooltipString, w - 10, 17, (GLfloat)(95 / 255.0f), (GLfloat)(95 / 255.0f), (GLfloat)(95 / 255.0f), alpha);
}