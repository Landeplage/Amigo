#pragma once
#include <GL\glew.h>
#include "Menu.h"
#include "MenuItem.h"
#include "Box.h"
#include "Button.h"
#include "Sprite.h"
#include "Font.h"
#include "RenderTarget.h"
#include <vector>
#include <functional>

class MenuSystem
{
public:
	MenuSystem(Sprite* sprCursor, Sprite* sprUI, Font* fontBold, Font* fontRegular);
	~MenuSystem();

	Menu* AddMenu(GLint x, GLint y, GLint width, GLint height);

	void Update(GLdouble time);
	void Draw();
	
	GLfloat Wobble(GLfloat currentX, GLfloat targetX, GLfloat currentSpeed, GLfloat force, GLfloat friction, GLdouble time);
	bool SetFocus(MenuItem* menuItem);
	void ResetFocus();
	MenuItem* GetFocus();
	void SetCursor(GLint cursorOffset);

	GLfloat GetRot();
	Font* GetFontBold();
	Sprite* GetSpriteUI();
	GLfloat GetOverlaySlide();
	bool GetOverlayShow();

	void ShowMessage(std::string title, std::string text, std::function<void()> onButton1);
	void OnButton1();

private:
	void HideOverlay();

	std::vector<Menu*> menus;
	MenuItem* focusedItem;

	Sprite *sprUI, *sprCursor;
	Font *fontBold, *fontRegular;
	GLfloat rot, overlaySlide, overlaySlideSpeed, overlaySlideTarget;
	GLint cursorOffset;

	RenderTarget *menuRenderTarget, *overlayRenderTarget;
	bool overlayShow;
	std::string overlayText;
	std::vector<MenuItem*> overlayItems;
	Box *overlayBox;
	std::function<void()> onButton1;
	Button *overlayButton1;
};