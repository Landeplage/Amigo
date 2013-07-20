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

	void Update(GLdouble time);
	void Draw();
	
	bool SetFocus(MenuItem* menuItem);
	void ResetFocus();
	MenuItem* GetFocus();

	void SetCursor(GLint cursorOffset);
	void SetTooltip(Vec2 position, std::string tooltipString);
	void ResetTooltip();

	GLfloat GetRot();
	Font* GetFontBold();
	Font* GetFontRegular();
	Sprite* GetSpriteUI();
	GLfloat GetOverlaySlide();
	bool GetOverlayShow();

	void MenuSystem::OverlayInit(std::string title, std::string text);
	void ShowMessage(std::string title, std::string text);
	void ShowMessage(std::string title, std::string text, std::function<void()> onButton1);
	void ShowQuestion(std::string title, std::string question, std::function<void()> onButton1, std::function<void()> onButton2);

private:
	void HideOverlay();
	void DrawTooltip();
	void OnOverlayButton1();
	void OnOverlayButton2();

	std::vector<Menu*> menus;
	MenuItem* focusedItem;

	Sprite *sprUI, *sprCursor;
	Font *fontBold, *fontRegular;
	GLfloat rot, overlaySlide;
	GLint cursorOffset;

	bool overlayShow;
	std::string overlayText;
	std::vector<MenuItem*> overlayItems;
	Box *overlayBox;
	std::function<void()> onOverlayButton1, onOverlayButton2;
	Button *overlayButton1, *overlayButton2;

	std::string tooltipString;
	GLdouble tooltipTimer;
	Vec2 tooltipPosition, tooltipSize;
};