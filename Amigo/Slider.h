#pragma once
#include "MenuItem.h"
#include "Button.h"
#include "Font.h"
#include "Vec2.h"
#include <functional>

class MenuSystem;

class Slider : public MenuItem
{
public:
	Slider();
	Slider(MenuSystem *menuSystem, std::string text, Vec2 position, GLint width, GLfloat min, GLfloat max, GLfloat step, GLint menuID, std::function<void()> onRelease);

	void Update(GLdouble time);
	virtual void Draw(GLfloat transition);
	void Unload();

	virtual void SetValue(GLfloat value);
	void SetMin(GLint min);
	void SetMax(GLint max);
	GLfloat GetValue();

protected:
	void Init();
	void HandleButton(Button *button, GLdouble time, GLint limitXLeft, GLint limitXRight, GLint maxPos, GLint minPos, GLfloat *value);
	void CalculateValue(Button *button, GLint maxPos, GLint minPos, GLfloat *value);
	void SnapButton(Button *button, GLfloat *value, GLint limitXLeft, GLint limitXRight, GLint maxPos, GLint minPos);
	void DrawCommonElements(GLint x, GLint y, GLfloat alpha);

	std::string text;
	MenuSystem *menuSystem;
	GLfloat value, min, max, step;
	Vec2 mouseOffset;
	Button *button;
	Font *font;
	std::string valueString;
	std::function<void()> onRelease;
};