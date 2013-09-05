#pragma once
#include <GL\glew.h>
#include "Menu.h"
#include "MenuItem.h"
#include "Box.h"
#include "Button.h"
#include "Sprite.h"
#include "Font.h"
#include "Vec2.h"
#include <vector>
#include <functional>

class MenuSystem
{
public:
	MenuSystem(Sprite* sprCursor, Sprite* sprUI, Font* fontBold, Font* fontRegular);
	~MenuSystem();

	Menu* AddMenu(GLint x, GLint y, GLint width, GLint height, GLint defaultMenuID);

	void HandleInput();
	void Update(GLdouble time);
	void Draw();
	
	bool SetFocus(MenuItem* menuItem);
	void ResetFocus();
	MenuItem* GetFocus();

	void SetCurrentScrollboxFocus(MenuItem* scrollbox);
	void ResetCurrentScrollboxFocus();
	MenuItem* GetCurrentScrollboxFocus();

	void SetCursor(GLint cursorOffset);
	void SetTooltip(Vec2 position, std::string tooltipString);
	void ResetTooltip();

	GLfloat GetRot();
	Font* GetFontBold();
	Font* GetFontRegular();
	Sprite* GetSpriteUI();
	GLfloat GetOverlaySlide();
	bool GetOverlayShow();

	void QueueMessage(std::string title, std::string text);
	void QueueMessage(std::string title, std::string text, std::function<void()> onButton1);
	void QueueQuestion(std::string title, std::string question, std::function<void()> onButton1, std::function<void()> onButton2);

private:
	void OverlayInit(std::string title, std::string text);
	void HideOverlay();
	void RenderOverlay();
	void DrawTooltip();
	void OnOverlayButton1();
	void OnOverlayButton2();

	std::vector<Menu*> menus;
	MenuItem* focusedItem;

	Sprite *sprUI, *sprCursor;
	Font *fontBold, *fontRegular;
	GLfloat rot;
	GLint cursorOffset;

	RenderTarget *overlayRenderTarget;
	bool overlayShow;
	GLfloat overlaySlide, overlayBackgroundAlpha, overlayBackgroundAlphaTarget;
	std::string overlayText;
	std::vector<MenuItem*> overlayItems;
	Box *overlayBox;
	std::function<void()> onOverlayButton1, onOverlayButton2;
	Button *overlayButton1, *overlayButton2;

	std::string tooltipString;
	GLdouble tooltipTimer;
	Vec2 tooltipPosition, tooltipSize;

	// Members for handling popup messages
	enum MSG_TYPE
	{
		MSG_MESSAGE,
		MSG_QUESTION
	};

	struct Message
	{
		Message(MSG_TYPE messageType, std::string title, std::string message, std::function<void()> onButton1, std::function<void()> onButton2)
		{
			this->messageType = messageType;
			this->title = title;
			this->message = message;
			this->onButton1 = onButton1;
			this->onButton2 = onButton2;
		}

		MSG_TYPE messageType;
		std::string message, title;
		std::function<void()> onButton1, onButton2;

	};

	std::vector<Message*> messageQueue;

	void ShowMessage(Message *message);
	void ShowQuestion(Message *message);

	// Members for handling wheel-scrolling in boxes
	MenuItem* currentScrollboxFocus;
};