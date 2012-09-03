#pragma once
#include "MenuItem.h"
#include "Font.h"
#include <functional>
#include "Vec2.h"

class MenuSystem;

class Button : public MenuItem
{
public:
	Button(MenuSystem* menuSystem, std::string text, GLint x, GLint y, GLint w, GLint h, MenuItem::Align align, GLint menuID, std::string tooltip, std::function<void()> onClick);
	
	virtual void Update(GLdouble time);
	virtual void Draw(GLfloat transition);
	virtual void Unload();

	void SetText(std::string text, MenuItem::Align align);
	void SetTextAlignment(MenuItem::Align align);
	void SetOnClick(std::function<void()> onClick);
	void SetMenuOffset(GLint xOffset, GLint yOffset);

	bool GetHasBeenClicked();

protected:
	virtual void onClick();

	Font* font;
	std::string text, tooltip;
	GLint state; // determines if button is pressed or highlighted
	std::function<void()> function;
	MenuSystem* menuSystem;
	Vec2 textOffset, menuOffset;
	bool hasBeenClicked;
};