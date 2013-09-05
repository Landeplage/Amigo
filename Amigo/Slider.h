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

	void HandleInput();
	void Update(GLdouble time);
	virtual void Draw();
	void Unload();

	virtual void SetValue(GLfloat value);
	void SetMin(GLint min);
	void SetMax(GLint max);

	GLfloat GetValue();
	MenuItem* GetButton();

	void SetSize(Vec2 size);
	void SetPosition(Vec2 position);
	void SetOrigin(Vec2 origin);
	void SetDrawOffset(Vec2 drawOffset);

protected:
	void Init();
	void HandleButton(Button *button, GLint limitXLeft, GLint limitXRight, GLint maxPos, GLint minPos, GLfloat *value);
	void CalculateValue(Button *button, GLint maxPos, GLint minPos, GLfloat *value);
	void SnapButton(Button *button, GLfloat *value, GLint limitXLeft, GLint limitXRight, GLint maxPos, GLint minPos);
	void LimitButton(Button *button, GLint limitXLeft, GLint limitXRight);
	void DrawCommonElements(GLint x, GLint y, GLfloat alpha);

	std::string text;
	GLfloat value, min, max, step;
	Vec2 mouseOffset;
	Button *button;
	Font *font;
	std::string valueString;
	std::function<void()> onRelease;
};