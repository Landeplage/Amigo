#pragma once
#include <GL\glew.h>
#include "MenuItem.h"
#include "Sprite.h"
#include "Font.h"
#include "Vec2.h"
#include <vector>
#include <functional>
#include "RenderTarget.h"

#define MENU_HISTORY_LENGTH 10

class MenuSystem; // forward-declaring MenuSystem to avoid including it

class Menu
{
public:
	Menu(GLint x, GLint y, GLint width, GLint height, MenuSystem* menuSystem, GLint defaultMenuID);
	~Menu();

	MenuItem* AddBox(std::string title, GLint x, GLint y, GLint width, GLint height, GLint menuID);
	MenuItem* AddContentbox(std::string title, GLint x, GLint y, GLint width, GLint height, GLint menuID);
	MenuItem* AddButton(std::string text, GLint x, GLint y, GLint width, GLint height, MenuItem::Align align, GLint menuID, std::string tooltip, std::function<void()> onClick);
	MenuItem* AddSlider(std::string text, Vec2 position, GLint width, GLfloat min, GLfloat max, GLfloat step, GLint menuID, std::function<void()> onRelease);
	MenuItem* AddSliderRange(std::string text, Vec2 position, GLint width, GLfloat min, GLfloat max, GLfloat step, GLint menuID, std::function<void()> onRelease);

	void HandleInput();
	void Update(GLdouble time);
	void Draw();
	void Render();
	void GoTo(int menuID);
	void GoToPrevious();

	GLint GetMenuCurrent();
	GLint GetMenuGoTo();
	GLfloat GetSlide();
	Vec2 GetPosition();
	GLint GetHistory(GLint id);

	void SetTransition(GLint moveX, GLint moveY, GLfloat scale);
	void OnDraw(std::function<void()> onDraw);

private:
	// Rendertarget stuff
	RenderTarget* renderTarget;
	
	// Functions
	void AddHistory(GLint menuID);
	void GoBackInHistory();

	// Properties
	std::function<void()> onDraw;
	bool active;
	Vec2 position;
	MenuSystem* menuSystem;
	std::vector<MenuItem*> items;
	Sprite* sprite;
	Font* fontBold;
	GLfloat slide, slideTarget, scale;
	GLint menuCurrent, menuGoTo, width, height, moveX, moveY;
	GLint history[MENU_HISTORY_LENGTH];
};