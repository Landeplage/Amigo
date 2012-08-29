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

	// Add items to overlay
	overlayItems.push_back(new Box(this, "", 10, 10, 10, 10, 0));
	overlayBox = (Box*)overlayItems[overlayItems.size() - 1];
	overlayItems.push_back(new Button(this, "", 0, 0, 0, 0, MenuItem::Align::CENTER, 0, [](){}, Vec2(0, 0)));
	overlayButton1 = (Button*)overlayItems[overlayItems.size() - 1];
	overlayItems.push_back(new Button(this, "", 0, 0, 0, 0, MenuItem::Align::CENTER, 0, [](){}, Vec2(0, 0)));
	overlayButton2 = (Button*)overlayItems[overlayItems.size() - 1];

	blend1 = 0;
	blend2 = 0;
}

MenuSystem::~MenuSystem()
{
	printf("Deleting MenuSystem...\n");

	// Delete all menus
	for(int i = 0; i < menus.size(); i ++)
	{
		menus[i]->~Menu();
	}

	// Delete rendertargets
	overlayRenderTarget->~RenderTarget();
	menuRenderTarget->~RenderTarget();

	// Delete overlay-items
	for(int i = 0; i < overlayItems.size(); i ++)
	{
		overlayItems[i]->Unload();
	}
}

void MenuSystem::Update(GLdouble time)
{
	// Pulsing variable for various effects
	rot += time * 0.01f;
	if (rot >= 360)
		rot -= 360;

	// Reset cursor offset
	cursorOffset = 0;

	// Update all menus
	for(int i = 0; i < menus.size(); i ++)
	{
		menus[i]->Update(time);
	}

	// Handle overlay-menus
	if (abs(overlaySlide) < 0.1f && overlayShow)
	{
		// Update overlay items
		for(int i = 0; i < overlayItems.size(); i ++)
		{
			overlayItems[i]->Update(time);
		}
	}

	// Slide-variable for the overlay
	overlaySlideSpeed = Wobble(overlaySlide, overlaySlideTarget, overlaySlideSpeed, 0.1f, 0.1f, time);
	overlaySlide += overlaySlideSpeed * time;
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
}

void MenuSystem::Draw()
{
	// Draw debug-text
	std::string str = toString(Input::getMouseWheel()) + " x " + toString(cursorOffset);
	fontBold->Draw((Context::getWindowWidth() - fontBold->GetWidth(str)) / 2, 10, str);

	// Begin menu-rendertarget
	menuRenderTarget->Begin();

	// Draw menus
	for(int i = 0; i < menus.size(); i ++)
	{
		menus[i]->Draw();
	}

	// End menu-rendertarget
	menuRenderTarget->End();

	// Set blend mode
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	// Draw menu-rendertarget
	GLfloat mod = (abs(overlaySlide) * 0.5f) + 0.5f;
	menuRenderTarget->Draw(0, 0, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, mod, 0, 0, menuRenderTarget->GetSize().x, menuRenderTarget->GetSize().y);
	// Reset blend mode
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Draw overlay for messages/questions
	if (abs(overlaySlide + 1) > 0)
	{
		// Set blend mode
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		// Black transparent fullscreen box
		sprUI->Draw(0, 0, 0.0f, Context::getWindowWidth(), Context::getWindowHeight(), 0.0f, 0.0f, 0.0f, 0.35f * (1 - abs(overlaySlide)), 49, 10, 1, 1);
		// Reset blend mode
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Rendertarget begin
		overlayRenderTarget->Begin();

		// Items of the overlay
		for(int i = 0; i < overlayItems.size(); i ++)
		{
			overlayItems[i]->Draw();
		}

		// Message-text
		fontRegular->DrawLinebreak(overlayBox->GetPosition().x + 10, overlayBox->GetPosition().y + 10, overlayText, overlayBox->GetSize().x - 20, 18);

		// Rendertarget end
		overlayRenderTarget->End();

		// Set blend mode
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	
		// Draw rendertarget
		GLfloat overlayScale = (abs(overlaySlide) + 1) * 0.2f + 0.8f;
		overlayRenderTarget->Draw((Context::getWindowWidth() - overlayRenderTarget->GetSize().x * overlayScale) / 2,
			-(overlayRenderTarget->GetSize().y * (overlayScale - 1)) / 2, 0.0f, overlayScale, overlayScale, 1.0f, 1.0f, 1.0f, 1 - abs(overlaySlide), 0, 0, overlayRenderTarget->GetSize().x, overlayRenderTarget->GetSize().y);
		
		// Reset blend mode
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	// Draw cursor
	if (cursorOffset > -1)
	{
		Vec2 mouse = Input::getMousePos();
		sprCursor->Draw(mouse.x, mouse.y, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, cursorOffset * 32, 0, 32, 32);
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
	if (currentX < targetX) { currentSpeed += (targetX - currentX) * force * 0.01; }
	else if (currentX > targetX) { currentSpeed -= (currentX - targetX) * force * 0.01; }
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
	overlayButton1->SetText("Okay", MenuItem::Align::CENTER);
	overlayButton1->SetPosition(overlayBox->GetPosition().x + overlayBox->GetSize().x / 2 - 100, overlayBox->GetPosition().y + overlayBox->GetSize().y + 15);
	overlayButton1->SetMenuOffset((Context::getWindowWidth() - overlayRenderTarget->GetSize().x) / 2, 0);
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
	overlayButton1->SetText("Yes", MenuItem::Align::CENTER);
	overlayButton1->SetPosition(overlayBox->GetPosition().x, overlayBox->GetPosition().y + overlayBox->GetSize().y + 15);
	overlayButton1->SetMenuOffset((Context::getWindowWidth() - overlayRenderTarget->GetSize().x) / 2, 0);
	overlayButton1->SetOnClick([=](){ this->OnButton1(); });
	this->onButton1 = onButton1;

	// Setup "No" button
	overlayButton2->SetSize(195, 35);
	overlayButton2->SetText("No", MenuItem::Align::CENTER);
	overlayButton2->SetPosition(overlayBox->GetPosition().x + overlayBox->GetSize().x - overlayButton2->GetSize().x, overlayBox->GetPosition().y + overlayBox->GetSize().y + 15);
	overlayButton2->SetMenuOffset((Context::getWindowWidth() - overlayRenderTarget->GetSize().x) / 2, 0);
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
	overlayBox->SetPosition(10, (Context::getWindowHeight() - overlayBox->GetSize().y) / 2);
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