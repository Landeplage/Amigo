#pragma once
#include <GL\glew.h>
#include "MenuItem.h"
#include "Sprite.h"
#include "Font.h"
#include "Vec2.h"
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
	MenuItem* AddSlider(std::string text, Vec2 position, GLint width, GLfloat min, GLfloat max, GLint menuID, std::function<void()> onRelease);

	void Update(GLdouble time);
	void Draw();
	void Render();
	void GoTo(int menuID);

	GLint GetMenuCurrent();
	GLfloat GetSlide();
	Vec2 GetPosition();

	void SetTransition(GLint moveX, GLint moveY, GLfloat scale, bool fancyEffect);
	void OnDraw(std::function<void()> onDraw);

private:
	// Rendertarget stuff
	RenderTarget* renderTarget;
	
	// Properties
	std::function<void()> onDraw;
	bool active, fancyEffect;
	Vec2 position;
	MenuSystem* menuSystem;
	std::vector<MenuItem*> items;
	Sprite* sprite;
	Font* fontBold;
	GLfloat slide, slideTarget, slideSpeed, scale;
	GLint menuCurrent, menuGoTo, width, height, moveX, moveY;
};