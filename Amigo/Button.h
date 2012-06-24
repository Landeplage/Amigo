#pragma once
#include "MenuItem.h"
#include "Sprite.h"
#include "Font.h"
#include <functional>
#include "Vec2.h"

class MenuSystem;

class Button : public MenuItem
{
public:
	Button(MenuSystem* menuSystem, std::string text, GLint x, GLint y, GLint w, GLint h, MenuItem::Align align, GLint menuID, std::function<void()> onClick, Vec2 menuOffset);
	
	virtual void Update(GLdouble time);
	virtual void Draw();
	virtual void Unload();

	void SetText(std::string text, MenuItem::Align align);
	void SetTextAlignment(MenuItem::Align align);
	void SetOnClick(std::function<void()> onClick);
	void SetMenuOffset(GLint xOffset, GLint yOffset);

protected:
	virtual void onClick();

	Sprite* sprite;
	Font* font;
	std::string text;
	GLint state; // determines if button is pressed or highlighted
	std::function<void()> function;
	MenuSystem* menuSystem;
	bool hasBeenClicked;
	Vec2 textOffset, menuOffset;
};