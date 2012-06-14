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
	overlayRenderTarget = new RenderTarget(420, Context::getWindowHeight(), 1);
	menuRenderTarget = new RenderTarget(Context::getWindowWidth(), Context::getWindowHeight(), 1);
	onButton1 = [](){};

	// Add items to overlay
	overlayItems.push_back(new Box(this, "", 10, 10, 10, 10, 0));
	overlayBox = (Box*)overlayItems[overlayItems.size() - 1];
	overlayItems.push_back(new Button(this, "", 0, 0, 0, 0, MenuItem::Align::CENTER, 0, [](){}, Point(0, 0)));
	overlayButton1 = (Button*)overlayItems[overlayItems.size() - 1];
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
	// Reset cursor offset
	cursorOffset = 0;

	// Update all menus
	for(int i = 0; i < menus.size(); i ++)
	{
		menus[i]->Update(time);
	}

	// Handle menus
	if (abs(overlaySlide) < 0.1f && overlayShow)
	{
		// Update overlay items
		for(int i = 0; i < overlayItems.size(); i ++)
		{
			overlayItems[i]->Update(time);
		}
	}

	// Pulsing variable for various effects
	rot += time * 0.01f;
	if (rot >= 360)
		rot -= 360;

	// Slide variable for the overlay
	overlaySlideSpeed = Wobble(overlaySlide, overlaySlideTarget, overlaySlideSpeed, 0.05f, 0.05f, time);
	overlaySlide += overlaySlideSpeed;
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
	fontBold->Draw(10, 10, toString(overlaySlide));

	// Render the menus to their rendertargets
	for(int i = 0; i < menus.size(); i ++)
	{
		menus[i]->Render();
	}

	// Begin menu-rendertarget
	menuRenderTarget->Begin();

	// Draw menus
	for(int i = 0; i < menus.size(); i ++)
	{
		menus[i]->Draw();
	}

	// End menu-rendertarget
	menuRenderTarget->End();

	// Draw menu-rendertarget
	GLfloat mod = (abs(overlaySlide) * 0.5f) + 0.5f;
	menuRenderTarget->Draw(0, 0, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, mod, 0, 0, menuRenderTarget->GetSize().x, menuRenderTarget->GetSize().y);

	// Draw overlay for messages/questions
	if (overlayShow || (overlaySlide < 1 &&  overlaySlide > 0))
	{
		// Black transparent fullscreen box
		sprUI->Draw(0, 0, 0.0f, Context::getWindowWidth(), Context::getWindowHeight(), 0.0f, 0.0f, 0.0f, 0.35f * (1 - abs(overlaySlide)), 49, 10, 1, 1);

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

		// Draw rendertarget
		overlayRenderTarget->Draw((Context::getWindowWidth() - overlayRenderTarget->GetSize().x) / 2 + (overlaySlide * 100),
			0, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1 - abs(overlaySlide), 0, 0, overlayRenderTarget->GetSize().x, overlayRenderTarget->GetSize().y);
	}

	// Draw cursor
	Point mouse = Input::getMousePos();
	sprCursor->Draw(mouse.x, mouse.y, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, cursorOffset * 32, 0, 32, 32);

	// Debug
	std::string str = "I realise the answer to this may be inherrently non-portable, but I don't quite understand GL_LUMIgghjyNANCE.";
	sprUI->Draw(0, 0, 0.0f, 200, fontRegular->GetHeight(str, 200, 18), 0.0f, 0.0f, 0.0f, 0.5f, 49, 10, 1, 1);
	fontRegular->DrawLinebreak(0, 0, str, 200, 18);
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
	if (currentX < targetX) { currentSpeed += (targetX - currentX) * force * time * 0.01; }
	else if (currentX > targetX) { currentSpeed -= (currentX - targetX) * force * time * 0.01; }
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

// Set cursor offset
void MenuSystem::SetCursor(GLint cursorOffset)
{
	this->cursorOffset = cursorOffset;
}

// Show a message-overlay on screen
void MenuSystem::ShowMessage(std::string title, std::string text, std::function<void()> onButton1)
{
	GLint centerX, centerY;
	centerX = Context::getWindowWidth() / 2;
	centerY = Context::getWindowHeight() / 2;

	overlayText = text;

	// Setup box
	overlayBox->SetTitle(title);
	overlayBox->SetSize(400, fontRegular->GetHeight(overlayText, 400 - 20, 18) + 20);
	overlayBox->SetPosition(10, centerY - 200 / 2);

	// Setup "Okay"-button
	overlayButton1->SetSize(200, 35);
	overlayButton1->SetText("Okay", MenuItem::Align::CENTER);
	overlayButton1->SetPosition(overlayBox->GetPosition().x + overlayBox->GetSize().x / 2 - 100, overlayBox->GetPosition().y + overlayBox->GetSize().y + 15);
	overlayButton1->SetMenuOffset(centerX - overlayRenderTarget->GetSize().x / 2, 0);
	overlayButton1->SetOnClick([=](){ this->OnButton1(); });
	this->onButton1 = onButton1;

	overlayShow = true;
	overlaySlideTarget = 0;
	ResetFocus();
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