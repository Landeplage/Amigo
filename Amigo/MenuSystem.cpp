#include "MenuSystem.h"
#include "Helper.h"
#include <math.h>
#include "Input.h"
#include "Context.h"

const Vec2
	OVERLAY_BUTTONSIZE = Vec2(102, 28),
	OVERLAY_MAX_BOXSIZE = Vec2(400, 200);

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
	overlayRenderTarget = new RenderTarget(0, 0);
	overlayItemsRenderTarget = new RenderTarget(0, 0);
	overlayItemsRenderTargetPosition = Vec2(0, 0);
	overlayShow = false;
	overlayText = "?";
	overlaySlide = -1.0f;
	overlayBackgroundAlpha = 0.0f;
	overlayBackgroundAlphaTarget = 0.0f;
	onOverlayButton1 = [](){};
	onOverlayButton2 = [](){};

	// Add elements to message overlay system

	// Box
	overlayBox = new Box(this, "", 0, 0, 0, 0, 0);
	overlayBox->visible = false;

	// Button 1
	overlayItems.push_back(new Button(this, "", 0, 0, 0, 0, MenuItem::CENTER, 0, "", [](){}));
	overlayButton1 = (Button*)overlayItems[overlayItems.size() - 1];
	overlayButton1->SetOnClick([=](){ this->OnOverlayButton1(); });

	// Button 2
	overlayItems.push_back(new Button(this, "", 0, 0, 0, 0, MenuItem::CENTER, 0, "", [](){}));
	overlayButton2 = (Button*)overlayItems[overlayItems.size() - 1];
	overlayButton2->SetOnClick([=](){ this->OnOverlayButton2(); });

	// Text item
	overlayTextItem = new Text(this, "", Vec2(0, 0), fontRegular, 0);
	overlayTextItem->SetColor(Color(255, 255, 255));
	overlayTextItem->SetShadowAttributes(Color(0, 0, 0), 0.3f);

	// Current scrollbox variables
	currentScrollboxFocus = 0;

	// Initialize dropdown-variables
	activeDropdown = NULL;

	// Initialize input-field variables
	activeInputField = NULL;
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

	// Unload overlay rendertarget
	overlayRenderTarget->~RenderTarget();

	// Unload overlay text-item
	overlayTextItem->Unload();
}

void MenuSystem::HandleInput()
{
	// Handle input on dropdowns
	if (IsDropdownActive())
	{
		// This handles input on the items of the current dropdown
		activeDropdown->HandleItems();

		// Make sure dropdown is deactivated/reset if the user clicks somewhere
		if (Input::getMouseLeftReleased())
		{
			activeDropdown->toggleDropdownStatus();
			ResetFocus();
		}
	}

	// Handle deactivation of the active input-field
	if (activeInputField != NULL)
	{
		if (Input::getMouseLeftPressed() && focusedItem != activeInputField)
		{
			activeInputField->ToggleInputActive();
			activeInputField = NULL;
		}
	}

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
}

void MenuSystem::Draw()
{
	// Get window width/height
	Vec2 wSize;
	wSize.x = Context::getWindowWidth();
	wSize.y = Context::getWindowHeight();

	// Draw debug-text
	std::string str = toString((GLint)activeDropdown);
	fontBold->Draw((Context::getWindowWidth() - fontBold->GetWidth(str)) / 2, 10, str);

	// Draw menus
	for(GLuint i = 0; i < menus.size(); i ++)
	{
		menus[i]->Draw();
	}

	// Draw the dropdown-list of the currently active dropdown
	if (IsDropdownActive())
	{
		activeDropdown->DrawItems();
	}

	// Draw black rectangle to dim background of overlay
	if (overlayBackgroundAlpha > 0.0f)
	{
		GLfloat alpha = overlayBackgroundAlpha * 0.5f;
		sprUI->Draw(0, 0, 0.0f, wSize.x, wSize.y, Color(0, 0, 0), alpha, 0, 72, 1, 1);
	}

	// Draw the overlay
	if (overlaySlide > -1.0f)
	{
		GLfloat slide, alpha;
		if (overlayShow)
			slide = EaseQuadIn(overlaySlide);
		else
			slide = EaseQuadIn(overlaySlide * -1);
		alpha = abs(overlaySlide + 1.0f);
		overlayRenderTarget->Draw((wSize.x - overlayRenderTarget->GetSize().x) / 2 + slide * 50, (wSize.y - overlayRenderTarget->GetSize().y) / 2, alpha);

		// Draw the items of the overlay
		if (overlaySlide == 0)
		{
			for (GLint i = 0; i < (GLint)overlayItems.size(); i++)
			{
				overlayItems[i]->Draw();
			}
		}
		else // draw the items-rendertarget
		{
			overlayItemsRenderTarget->Draw(overlayItemsRenderTargetPosition.x + slide * 50, overlayItemsRenderTargetPosition.y, alpha);
		}
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
	// Update rendertarget size to match box size
	overlayRenderTarget->SetSize(overlayBox->GetSize() + Vec2(2, 0));

	overlayRenderTarget->Begin();

	float alpha;
	Color col;
	int x, y, w, h;

	// Draw background box
	alpha = 1.0f;
	col = Color(122, 122, 122);
	x = 0;
	y = 0;
	w = (int)overlayBox->GetSize().x;
	h = (int)overlayBox->GetSize().y;

	// Draw box
	sprUI->DrawRectangleFromTexture(Vec2(x, y), Vec2(w + 2, h), Vec2(25, 45), Vec2(5, 7), Vec2(1, 17), col);

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

	// Title
	fontBold->Draw((GLint)(x + overlayBox->GetSize().x / 2 - fontBold->GetWidth(overlayBox->GetTitle()) / 2),
		y + 25 + 1,
		overlayBox->GetTitle(), 0.0f, 1.0f, 1.0f,
		Color(0, 0, 0), 0.5f); // shadow
	fontBold->Draw((GLint)(x + overlayBox->GetSize().x / 2 - fontBold->GetWidth(overlayBox->GetTitle()) / 2),
		y + 25,
		overlayBox->GetTitle(), 0.0f, 1.0f, 1.0f,
		Color(255, 255, 255), 1.0f); // actual text

	// Draw text
	overlayTextItem->Draw();

	overlayRenderTarget->End();
}

// Render the rendertarget that will be used for making it look like the buttons are sliding in
void MenuSystem::RenderOverlayItems()
{
	// Loop through all overlayItems, and find out how big our rendertarget needs to be
	Vec2 rtLeftTop, rtRightBot, tmpLeftTop, tmpRightBot;

	rtLeftTop = Vec2(Context::getWindowWidth(), Context::getWindowHeight()); // set LeftTop to a high value, so it doesn't get stuck at 0.0f.

	for (int i = 0; i < overlayItems.size(); i++)
	{
		if (!overlayItems[i]->active)
			continue;

		tmpLeftTop = overlayItems[i]->GetPosition();
		tmpRightBot = tmpLeftTop + overlayItems[i]->GetSize();

		// Find leftmost point
		if (tmpLeftTop.x < rtLeftTop.x)
			rtLeftTop.x = tmpLeftTop.x;

		// Find topmost point
		if (tmpLeftTop.y < rtLeftTop.y)
			rtLeftTop.y = tmpLeftTop.y;

		// Find rightmost point
		if (tmpRightBot.x > rtRightBot.x)
			rtRightBot.x = tmpRightBot.x;

		// Find downmost point
		if (tmpRightBot.y > rtRightBot.y)
			rtRightBot.y = tmpRightBot.y;
	}

	// Apply rendertarget size
	Vec2 padding;
	padding = Vec2(20, 20); // add some pixels for padding, since some items have graphics beyond their size
	overlayItemsRenderTarget->SetSize(rtRightBot - rtLeftTop + padding);

	// Now we also know where to draw our rendertarget
	overlayItemsRenderTargetPosition = rtLeftTop - Vec2(padding.x / 2, padding.y / 2); // move it to make up for the padding

	// Adjust rtLeftTop for padding
	rtLeftTop = rtLeftTop - Vec2(padding.x / 2, padding.y / 2);

	// Transform item positions from absolute to relative
	for (int i = 0; i < overlayItems.size(); i++)
	{
		overlayItems[i]->SetPosition(overlayItems[i]->GetPosition() - rtLeftTop);
	}

	// Prepare rendertarget for drawing
	overlayItemsRenderTarget->Begin();

	// Draw the items of the overlay
	for (GLint i = 0; i < (GLint)overlayItems.size(); i++)
	{
		overlayItems[i]->Draw();
	}

	// Perform render
	overlayItemsRenderTarget->End();

	// Transform item positions from relative to absolute
	for (int i = 0; i < overlayItems.size(); i++)
	{
		overlayItems[i]->SetPosition(overlayItems[i]->GetPosition() + rtLeftTop);
	}
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
	// Init overlay text-item and box
	OverlayInit(message->title, message->message);

	// Some helpful variables for setting up button
	Vec2 wSize, bSize;
	wSize = Vec2(Context::getWindowWidth(), Context::getWindowHeight());
	bSize = OVERLAY_BUTTONSIZE;

	// Setup "Okay"-button
	overlayButton1->SetSize(bSize);
	overlayButton1->SetText("Okay", MenuItem::CENTER);
	overlayButton1->SetPosition(Vec2((wSize.x - bSize.x) / 2, (wSize.y + overlayBox->GetSize().y) / 2 - bSize.y - 25));
	this->onOverlayButton1 = message->onButton1;

	// Hide the other button
	overlayButton2->active = false;
	overlayButton2->visible = false;

	// Render the overlay
	RenderOverlay();

	// Render the overlay-items rendertarget
	RenderOverlayItems();
}

// Show a question on screen, with two options
void MenuSystem::ShowQuestion(Message *message)
{
	// Init overlay text-item and box
	OverlayInit(message->title, message->message);

	// Some helpful variables for setting up buttons
	Vec2 wSize, bSize;
	wSize = Vec2(Context::getWindowWidth(), Context::getWindowHeight());
	bSize = OVERLAY_BUTTONSIZE;

	// Setup "Yes"-button
	overlayButton1->SetSize(bSize);
	overlayButton1->SetText("Yes", MenuItem::CENTER);
	overlayButton1->SetPosition(Vec2((wSize.x - bSize.x) / 2 - 70, (wSize.y + overlayBox->GetSize().y) / 2 - bSize.y - 25));
	this->onOverlayButton1 = message->onButton1;

	// Setup "No" button
	overlayButton2->SetSize(bSize);
	overlayButton2->SetText("No", MenuItem::CENTER);
	overlayButton2->SetPosition(Vec2((wSize.x - bSize.x) / 2 + 70, (wSize.y + overlayBox->GetSize().y) / 2 - bSize.y - 25));
	this->onOverlayButton2 = message->onButton2;

	// Make sure the "No" button is visible and active
	overlayButton2->active = true;
	overlayButton2->visible = true;

	// Render the overlay
	RenderOverlay();

	// Render the overlay-items rendertarget
	RenderOverlayItems();
}

// Initiate the overlay-box
void MenuSystem::OverlayInit(std::string title, std::string text)
{
	// Setup overlay
	overlayText = text;
	overlayShow = true;
	ResetFocus();

	// Update the text-item
	overlayTextItem->SetText(overlayText);
	overlayTextItem->SetMaxWidth(350);

	// Setup box
	overlayBox->SetTitle(title);
	Vec2 boxSize;
	boxSize = overlayTextItem->GetSize() + Vec2(100, 150);
	if (boxSize.x < OVERLAY_MAX_BOXSIZE.x)
		boxSize.x = OVERLAY_MAX_BOXSIZE.x;
	if (boxSize.y < OVERLAY_MAX_BOXSIZE.y)
		boxSize.y = OVERLAY_MAX_BOXSIZE.y;
	overlayBox->SetSize(boxSize);
	overlayBox->SetPosition(Vec2(0, (Context::getWindowHeight() - overlayBox->GetSize().y) / 2));

	// Update the text-item position, based on the box's width
	overlayTextItem->SetPosition(Vec2((boxSize.x - overlayTextItem->GetSize().x) / 2, 70));
}

// Queue a message
void MenuSystem::QueueMessage(std::string title, std::string message)
{
	QueueMessage(title, message, [](){});
}

// Queue a message with a function
void MenuSystem::QueueMessage(std::string title, std::string message, std::function<void()> onButton1)
{
	messageQueue.push_back(new Message(MSG_MESSAGE, title, message, onButton1, [](){}));
}

// Show a yes/no question
void MenuSystem::QueueQuestion(std::string title, std::string question, std::function<void()> onButton1, std::function<void()> onButton2)
{
	messageQueue.push_back(new Message(MSG_QUESTION, title, question, onButton1, onButton2));
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

// Activates a given dropdown
void MenuSystem::ActivateDropdown(Dropdown* dropdown)
{
	activeDropdown = dropdown;
}

// Sets activeDropdown to NULL, this deactivating it
void MenuSystem::DeactivateDropdown()
{
	activeDropdown = NULL;
}

// Gets whether or not focus is locked
bool MenuSystem::IsDropdownActive()
{
	if (activeDropdown == NULL)
		return false;
	else
		return true;
}

// Set an input field to be active (so that menuSystem can eventually deactivate it)
void MenuSystem::ActivateInputField(InputField *inputField)
{
	activeInputField = inputField;
}

// Get the currently active input-field (if any). If none is active, NULL is returned.
InputField* MenuSystem::GetActiveInputField()
{
	return activeInputField;
}