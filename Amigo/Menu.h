#pragma once
#include <GL\glew.h>
#include "MenuItem.h"
#include "Sprite.h"
#include "Font.h"
#include "Point.h"
#include <vector>
#include <functional>
#include "RenderTarget.h"

class MenuSystem; // forward-declaring MenuSystem to avoid including it

class Menu
{
public:
	Menu(GLint x, GLint y, GLint width, GLint height, MenuSystem* menuSystem);
	~Menu();

	MenuItem* AddBox(std::string title, GLint x, GLint y, GLint width, GLint height, GLint menuID);
	MenuItem* AddButton(std::string text, GLint x, GLint y, GLint width, GLint height, MenuItem::Align align, GLint menuID, std::function<void()> onClick);

	void Update(GLdouble time);
	void Draw();
	void Render();
	void GoTo(int menuID);

	GLint GetMenuCurrent();
	GLfloat GetSlide();
	Point GetPosition();

	void SetTransition(GLint moveX, GLint moveY, GLfloat scale, bool fancyEffect);
	void OnDraw(std::function<void()> onDraw);

private:
	// Rendertarget stuff
	RenderTarget* renderTarget;
	
	// Properties
	std::function<void()> onDraw;
	bool active, fancyEffect;
	Point position;
	MenuSystem* menuSystem;
	std::vector<MenuItem*> items;
	Sprite* sprite;
	Font* fontBold;
	GLfloat slide, slideTarget, slideSpeed, scale;
	GLint menuCurrent, menuGoTo, width, height, moveX, moveY;
};