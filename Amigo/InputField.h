#pragma once

#include "MenuItem.h"
#include <functional>
#include "Font.h"
#include "Button.h"

class InputField : public MenuItem
{
public:
	InputField(MenuSystem* menuSystem, Vec2 position, GLint width, std::string infoText, GLint menuID, std::function<void()> onEnter);

	virtual void Unload();
	virtual void HandleInput();
	virtual void Update(GLdouble time);
	virtual void Draw();

	void SetSize(Vec2 size);
	void SetPosition(Vec2 position);
	void SetOrigin(Vec2 origin);
	void SetDrawOffset(Vec2 drawOffset);

	void SetText(std::string text);
	std::string GetText();

	void ToggleInputActive();

private:
	Font *font;
	std::function<void()> onEnter;
	Button *button; // button used for obtaining focus/activating input
	std::string text, displayText, infoText;
	
	void UpdateDisplayText();

	// Input handling members
	bool inputActive;
	bool backspaceLast, leftArrowLast, rightArrowLast;
	int editPos;

	// Timer for input-indicator
	GLdouble indicatorTimer;
	void ResetIndicatorTimer();
};