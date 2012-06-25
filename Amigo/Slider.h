#pragma once
#include "MenuItem.h"
#include "Button.h"
#include "Font.h"
#include "Vec2.h"

class MenuSystem;

class Slider : public MenuItem
{
public:
	Slider(MenuSystem *menuSystem, std::string text, Vec2 position, GLint width, GLfloat min, GLfloat max, GLint menuID, std::function<void()> onRelease, Vec2 menuOffset);

	void Update(GLdouble time);
	void Draw();
	void Unload();

	GLfloat GetValue();

private:
	std::string text;
	MenuSystem *menuSystem;
	GLfloat value, min, max;
	Vec2 mouseOffset;
	Button *button;
	Font *font;
	std::function<void()> onRelease;
};