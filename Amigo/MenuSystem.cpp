#include "MenuSystem.h"
#include "Helper.h"
#include <math.h>
#include "Input.h"
#include "Context.h"

MenuSystem::MenuSystem(Sprite* sprCursor, Sprite* sprUI, Font* fontBold, Font* fontRegular)
{
	printf("-> Creating MenuSystem...\n");

	// Initialize variables
	ResetFocus();
	this->sprUI = sprUI;
	this->fontBold = fontBold;
	this->fontRegular = fontRegular;
	this->sprCursor = sprCursor;
	rot = 0;
	cursorOffset = 0;

	// Tooltip variables
	tooltipPosition = Vec2(0, 0);
	tooltipSize = Vec2(0, 0);
	tooltipString = "?";
	tooltipTimer = -1.0;

	// Overlay variables
	overlayRenderTarget = new RenderTarget(400, Context::getWindowHeight());
	overlayShow = false;
	overlayText = "?";
	overlaySlide = -1.0f;
	overlayBackgroundAlpha = 0.0f;
	overlayBackgroundAlphaTarget = 0.0f;
	onOverlayButton1 = [](){};
	onOverlayButton2 = [](){};

	// Add elements to message overlay system
	overlayItems.push_back(new Box(this, "", 10, 10, overlayRenderTarget->GetSize().x, 10, 0));
	overlayBox = (Box*)overlayItems[overlayItems.size() - 1];
	overlayBox->visible = false;
	overlayItems.push_back(new Button(this, "", 0, 0, 0, 0, MenuItem::CENTER, 0, "", [](){}));
	overlayButton1 = (Button*)overlayItems[overlayItems.size() - 1];
	overlayButton1->SetOnClick([=](){ this->OnOverlayButton1(); });
	overlayButton1->SetOrigin(Vec2((Context::getWindowWidth() - overlayRenderTarget->GetSize().x) / 2, 0));
	overlayItems.push_back(new Button(this, "", 0, 0, 0, 0, MenuItem::CENTER, 0, "", [](){}));
	overlayButton2 = (Button*)overlayItems[overlayItems.size() - 1];
	overlayButton2->SetOnClick([=](){ this->OnOverlayButton2(); });
	overlayButton2->SetOrigin(Vec2((Context::getWindowWidth() - overlayRenderTarget->GetSize().x) / 2, 0));

	// Current scrollbox variables
	currentScrollboxFocus = 0;
}

MenuSystem::~MenuSystem()
{
	printf("-> Deleting MenuSystem...\n");

	// Delete all menus
	for(GLuint i = 0; i < menus.size(); i ++)
	{
		menus[i]->~Menu();
	}

	// Delete overlay-items
	for(GLuint i = 0; i < overlayItems.size(); i ++)
	{
		overlayItems[i]->Unload();
	}
}

void MenuSystem::HandleInput()
{
	// Handle input on all menus
	for(GLuint i = 0; i < menus.size(); i ++)
	{
		menus[i]->HandleInput();
	}

	// Handle overlay-menus
	if (abs(overlaySlide) == 0.0f && overlayShow)
	{
		// Update overlay items
		for(GLuint i = 0; i < overlayItems.size(); i ++)
		{
			overlayItems[i]->HandleInput();
		}
	}
}

void MenuSystem::Update(GLdouble time)
{
	// Pulsing variable for various effects
	rot += (GLfloat)time * 0.01f;
	if (rot >= 360)
		rot -= 360;

	// Update all menus
	for(GLuint i = 0; i < menus.size(); i ++)
	{
		menus[i]->Update(time);
	}

	// Handle overlay-menus
	if (abs(overlaySlide) == 0.0f && overlayShow)
	{
		// Update overlay items
		for(GLuint i = 0; i < overlayItems.size(); i ++)
		{
			overlayItems[i]->Update(time);
		}
	}
	else
	{
		if (overlaySlide == -1)
		{
			// one-time event which happens when a message appears
			if (messageQueue.size() > 0)
			{
				switch(messageQueue[0]->messageType)
				{
					case MSG_MESSAGE: ShowMessage(messageQueue[0]); break;
					case MSG_QUESTION: ShowQuestion(messageQueue[0]); break;
				}
				overlayBackgroundAlphaTarget = 1.0f;
				tooltipTimer  = -1.0f;
			}
		}
	}

	// Overlay slide effect
	if (overlayShow)
	{
		if (overlaySlide < 0)
			overlaySlide += time / 250;
		else
			overlaySlide = 0;
	}
	else
	{
		if (overlaySlide > -1)
			overlaySlide -= time / 250;
		else
			overlaySlide = -1; 
	}

	// Overlay background dim
	if (overlayBackgroundAlpha > overlayBackgroundAlphaTarget)
	{
		overlayBackgroundAlpha -= time / 250.0;
		if (overlayBackgroundAlpha < 0.0f)
			overlayBackgroundAlpha = 0.0f;
	}
	else
	{
		if (overlayBackgroundAlpha < overlayBackgroundAlphaTarget)
		{
			overlayBackgroundAlpha += time / 250.0;
			if (overlayBackgroundAlpha > 1.0f)
				overlayBackgroundAlpha = 1.0f;
		}
	}

	// Tooltip-timer
	if (tooltipTimer > 0)
	{
		tooltipTimer -= time / 1000;
		if (tooltipTimer <= 0)
			tooltipTimer = 0;
	}

	// Render to the overlayrendertarget
	if (overlaySlide > -1.0f)
	{
		RenderOverlay();
	}
}

void MenuSystem::Draw()
{
	// Draw debug-text
	std::string str = toString(overlaySlide);
	fontBold->Draw((Context::getWindowWidth() - fontBold->GetWidth(str)) / 2, 10, str);

	// Draw menus
	for(GLuint i = 0; i < menus.size(); i ++)
	{
		menus[i]->Draw();
	}

	// Draw black rectangle to dim background of overlay
	if (overlayBackgroundAlpha > 0.0f)
	{
		GLfloat alpha = overlayBackgroundAlpha * 0.5f;
		sprUI->Draw(0, 0, 0.0f, (GLfloat)Context::getWindowWidth(), (GLfloat)Context::getWindowHeight(), Color(0, 0, 0), alpha, 0, 72, 1, 1);
	}

	// Draw the overlay render
	if (overlaySlide > -1.0f)
	{
		GLfloat slide, alpha;
		if (overlayShow)
			slide = EaseQuadIn(overlaySlide);
		else
			slide = EaseQuadIn(overlaySlide * -1);
		alpha = abs(overlaySlide + 1.0f);
		overlayRenderTarget->Draw((Context::getWindowWidth()  - overlayRenderTarget->GetSize().x) / 2 + slide * 50, 0, alpha);
	}

	// Draw tooltip
	if (tooltipTimer == 0)
		DrawTooltip();

	// Draw cursor
	if (cursorOffset > -1)
	{
		Vec2 mouse = Input::getMousePos();
		sprCursor->Draw((GLint)mouse.x, (GLint)mouse.y, 0.0f, 1.0f, 1.0f, 1.0f, cursorOffset * 32, 0, 32, 32);
	}
}

// Render stuff to overlayrendertarget
void MenuSystem::RenderOverlay()
{
	overlayRenderTarget->Begin();

	float alpha;
	Color col;
	int x, y, w, h;

	// Draw background box
	alpha = 1.0f;
	col = Color(122, 122, 122);
	x = (int)overlayBox->GetPosition().x;
	y = (int)overlayBox->GetPosition().y;
	w = (int)overlayBox->GetSize().x;
	h = (int)overlayBox->GetSize().y;
	sprUI->Draw(x - 1, y, 0.0f, 1.0f, 1.0f, col, alpha, 25, 45, 5, 4); // Left top corner
	sprUI->Draw(x - 1, y + 4, 0.0f, 1.0f, (GLfloat)(h - 8) / 20.0f, col, alpha, 25, 49, 5, 20); // Left side
	sprUI->Draw(x - 1, y + h - 4, 0.0f, 1.0f, 1.0f, col, alpha, 25, 69, 5, 7); // Left bottom corner
	sprUI->Draw(x + w - 4, y + h - 4, 0.0f, 1.0f, 1.0f, col, alpha, 31, 69, 5, 7); // Right bottom corner
	sprUI->Draw(x + w - 4, y + 4, 0.0f, 1.0f, (GLfloat)(h - 8) / 20.0f, col, alpha, 31, 49, 5, 20); // Left side
	sprUI->Draw(x + w - 4, y, 0.0f, 1.0f, 1.0f, col, alpha, 31, 45, 5, 4); // Right top corner
	sprUI->Draw(x + 4, y, 0.0f, (GLfloat)(w - 8), 1.0f, col, alpha, 30, 45, 1, 4); // Top
	sprUI->Draw(x + 4, y + 4, 0.0f, (GLfloat)(w - 8), (GLfloat)(h - 8) / 20.0f, col, alpha, 30, 49, 1, 20); // Mid
	sprUI->Draw(x + 4, y + h - 4, 0.0f, (GLfloat)(w - 8), 1.0f, col, alpha, 30, 69, 1, 7); // Bottom

	// Draw some noise
	int wTmp, hTmp, nW, nH;
	wTmp = 0;
	hTmp = 0;
	nW = 100;
	nH = 100;
	for(int n = 0; n < h; n += nH)
	{
		if (n < h - nH)
			hTmp = nH;
		else
			hTmp = h - n;

		for(int i = 0; i < w; i += nW)
		{
			if (i < w - nW)
				wTmp = nW;
			else
				wTmp = w - i;
			sprUI->Draw(x + i, y + n, 0.0f, 1.0f, 1.0f, alpha, 44, 0, wTmp, hTmp);
		}
	}

	// Items of the overlay
	for(GLint i = 0; i < (GLint)overlayItems.size(); i ++)
	{
		overlayItems[i]->Draw();
	}

	// Title
	fontBold->Draw((GLint)(x + overlayBox->GetSize().x / 2 - fontBold->GetWidth(overlayBox->GetTitle()) / 2),
		y + 25 + 1,
		overlayBox->GetTitle(), 0.0f, 1.0f, 1.0f,
		Color(0, 0, 0), 0.5f); // shadow
	fontBold->Draw((GLint)(x + overlayBox->GetSize().x / 2 - fontBold->GetWidth(overlayBox->GetTitle()) / 2),
		y + 25,
		overlayBox->GetTitle(), 0.0f, 1.0f, 1.0f,
		Color(255, 255, 255), 1.0f); // actual text

	// Message-text
	fontRegular->DrawLinebreak((GLint)(x + overlayBox->GetSize().x / 2 - fontRegular->GetWidth(overlayText, overlayBox->GetSize().x - 30) / 2),
		y + 70 - 1,
		overlayText, (GLint)overlayBox->GetSize().x - 30, 18,
		Color(0, 0, 0), 0.5f); // shadow
	fontRegular->DrawLinebreak((GLint)(x + overlayBox->GetSize().x / 2 - fontRegular->GetWidth(overlayText, overlayBox->GetSize().x - 30) / 2),
		y + 70,
		overlayText, (GLint)overlayBox->GetSize().x - 30, 18,
		Color(255, 255, 255), 1.0f); // actual text

	overlayRenderTarget->End();
}

// Add a menu
Menu* MenuSystem::AddMenu(GLint x, GLint y, GLint width, GLint height, GLint defaultMenuID)
{
	menus.push_back(new Menu(x, y, width, height, this, defaultMenuID));
	return menus[menus.size() - 1];
}

// Set focus
bool MenuSystem::SetFocus(MenuItem* menuItem)
{
	if (focusedItem == NULL)
	{
		focusedItem = menuItem;
		SetCursor(1);
		return true;
	}
	return false;
}

// Reset focus
void MenuSystem::ResetFocus()
{
	focusedItem = NULL;
	SetCursor(0);
}

// Get focused item
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
void MenuSystem::ShowMessage(Message *message)
{
	OverlayInit(message->title, message->message);

	// Setup "Okay"-button
	overlayButton1->SetSize(Vec2(102, 28));
	overlayButton1->SetText("Okay", MenuItem::CENTER);
	overlayButton1->SetPosition(Vec2(overlayBox->GetPosition().x + overlayBox->GetSize().x / 2 - 51, overlayBox->GetPosition().y + overlayBox->GetSize().y - 50));
	this->onOverlayButton1 = message->onButton1;

	// Hide the other button
	overlayButton2->active = false;
	overlayButton2->visible = false;
}

// Show a question on screen, with two options
void MenuSystem::ShowQuestion(Message *message)
{
	OverlayInit(message->title, message->message);

	// Setup "Yes"-button
	overlayButton1->SetSize(Vec2(102, 28));
	overlayButton1->SetText("Yes", MenuItem::CENTER);
	overlayButton1->SetPosition(Vec2(overlayBox->GetPosition().x + overlayBox->GetSize().x / 2 - 51 - 71, overlayBox->GetPosition().y + overlayBox->GetSize().y - 50));
	this->onOverlayButton1 = message->onButton1;

	// Setup "No" button
	overlayButton2->SetSize(Vec2(102, 28));
	overlayButton2->SetText("No", MenuItem::CENTER);
	overlayButton2->SetPosition(Vec2(overlayBox->GetPosition().x + overlayBox->GetSize().x / 2 - 51 + 71, overlayBox->GetPosition().y + overlayBox->GetSize().y - 50));
	this->onOverlayButton2 = message->onButton2;

	// Make sure the "No" button is visible and active
	overlayButton2->active = true;
	overlayButton2->visible = true;
}

// Queue a message
void MenuSystem::QueueMessage(std::string title, std::string message)
{
	messageQueue.push_back(new Message(MSG_MESSAGE, title, message, [](){}, [](){} ));
}

void MenuSystem::QueueMessage(std::string title, std::string message, std::function<void()> onButton1)
{
	messageQueue.push_back(new Message(MSG_MESSAGE, title, message, onButton1, [](){}));
}

// Show a yes/no question
void MenuSystem::QueueQuestion(std::string title, std::string question, std::function<void()> onButton1, std::function<void()> onButton2)
{
	messageQueue.push_back(new Message(MSG_QUESTION, title, question, onButton1, onButton2));
}

// Initiate the overlay-box
void MenuSystem::OverlayInit(std::string title, std::string text)
{
	// Setup overlay
	overlayText = text;
	overlayShow = true;
	ResetFocus();

	// Setup box
	overlayBox->SetTitle(title);
	overlayBox->SetSize(Vec2(overlayBox->GetSize().x, fontRegular->GetHeight(overlayText, 400 - 30, 18) + 150));
	if (overlayBox->GetSize().y < 250)
		overlayBox->SetSize(Vec2(overlayBox->GetSize().x, 225));
	overlayBox->SetPosition(Vec2(0, (Context::getWindowHeight() - overlayBox->GetSize().y) / 2));
	// (GLint)(Context::getWindowWidth() / 2 - overlayBox->GetSize().x / 2)
}

// Hide the overlay
void MenuSystem::HideOverlay()
{
	if (messageQueue.size() > 0)
	{
		messageQueue.erase(messageQueue.begin());
	}

	if (messageQueue.size() == 0)
	{
		overlayBackgroundAlphaTarget = 0.0f;
	}

	overlayShow = false;
	ResetFocus();
}

// Perform the action of overlay-Button 1
void MenuSystem::OnOverlayButton1()
{
	onOverlayButton1();
	HideOverlay();
}

// Perform the action of overlay-Button 2
void MenuSystem::OnOverlayButton2()
{
	onOverlayButton2();
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
	sprUI->Draw(x - 1, y, 0.0f, 1.0f, 1.0f, alpha, 25, 45, 5, 4); // Left top corner
	sprUI->Draw(x - 1, y + 4, 0.0f, 1.0f, (GLfloat)(h - 8) / 20.0f, alpha, 25, 49, 5, 20); // Left side
	sprUI->Draw(x - 1, y + h - 4, 0.0f, 1.0f, 1.0f, alpha, 25, 69, 5, 7); // Left bottom corner
	sprUI->Draw(x + w - 4, y + h - 4, 0.0f, 1.0f, 1.0f, alpha, 31, 69, 5, 7); // Right bottom corner
	sprUI->Draw(x + w - 4, y + 4, 0.0f, 1.0f, (GLfloat)(h - 8) / 20.0f, alpha, 31, 49, 5, 20); // Left side
	sprUI->Draw(x + w - 4, y, 0.0f, 1.0f, 1.0f, alpha, 31, 45, 5, 4); // Right top corner
	sprUI->Draw(x + 4, y, 0.0f, (GLfloat)(w - 8), 1.0f, alpha, 30, 45, 1, 4); // Top
	sprUI->Draw(x + 4, y + 4, 0.0f, (GLfloat)(w - 8), (GLfloat)(h - 8) / 20.0f, alpha, 30, 49, 1, 20); // Mid
	sprUI->Draw(x + 4, y + h - 4, 0.0f, (GLfloat)(pointerX - 4), 1.0f, alpha, 30, 69, 1, 7); // Bottom Left
	sprUI->Draw(x + pointerX + 22, y + h - 4, 0.0f, (GLfloat)(w - pointerX - 22 - 4), 1.0f, alpha, 30, 69, 1, 7); // Bottom Right
	sprUI->Draw(x + pointerX, y + h - 4, 0.0f, 22, 1.0f, alpha, 30, 69, 1, 3); // Patch above pointer
	sprUI->Draw(x + pointerX, y + h - 1, 0.0f, 1.0f, 1.0f, alpha, 18, 31, 22, 13); // Pointer

	// Tooltip text
	fontRegular->DrawLinebreak(x + 8, y + 8, tooltipString, w - 10, 17, Color(255, 255, 255), alpha);
	fontRegular->DrawLinebreak(x + 8, y + 7, tooltipString, w - 10, 17, Color(95, 95, 95), alpha);
}

void MenuSystem::SetCurrentScrollboxFocus(MenuItem* scrollbox)
{
	currentScrollboxFocus = scrollbox;
}

void MenuSystem::ResetCurrentScrollboxFocus()
{
	currentScrollboxFocus = 0;
}

MenuItem* MenuSystem::GetCurrentScrollboxFocus()
{
	return currentScrollboxFocus;
}