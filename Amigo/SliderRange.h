#pragma once
#include "Slider.h"

class SliderRange : public Slider
{
public:
	SliderRange(MenuSystem *menuSystem, std::string text, Vec2 position, GLint width, GLfloat min, GLfloat max, GLfloat step, GLint menuID, std::function<void()> onRelease);

	void Update(GLdouble time);
	void Draw(GLfloat transition);

	void SetValue(GLfloat value);

private:
	void Init();

	GLfloat value2;
	Button *button2;
};