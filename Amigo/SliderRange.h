#pragma once
#include "Slider.h"

class SliderRange : public Slider
{
public:
	SliderRange(MenuSystem *menuSystem, std::string text, Vec2 position, GLint width, GLfloat min, GLfloat max, GLfloat step, GLint menuID, std::function<void()> onRelease);

	void HandleInput();
	void Update(GLdouble time);
	void Draw();

	void SetValue(GLfloat value);
	void SetSize(Vec2 size);
	void SetPosition(Vec2 position);
	void SetOrigin(Vec2 origin);
	void SetDrawOffset(Vec2 drawOffset);

private:
	void Init();

	GLfloat value2;
	Button *button2;
};