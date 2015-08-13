#pragma once
#include "MenuItem.h"
#include "Font.h"
#include "Color.h"
#include "RenderTarget.h"

class Text : public MenuItem
{
public:
	Text(MenuSystem* menuSystem, std::string text, Vec2 position, Font* font, GLint menuID);

	virtual void Unload();
	virtual void HandleInput();
	virtual void Update(GLdouble time);
	virtual void Draw();

	void SetText(std::string text);
	void SetColor(Color color);
	void SetAlpha(GLfloat alpha);
	void SetMaxWidth(GLint maxWidth);
	void SetShadowAttributes(Color color, GLfloat alpha);

	enum TextMode
	{
		LINEBREAK,
		TRUNCATE
	};

	void SetTextMode(TextMode textMode);
	void SetAlign(Align align);

private:
	RenderTarget *rt;
	Font* font;
	std::string text;
	Color color, shadowColor;
	GLfloat alpha, shadowAlpha;
	GLint maxWidth;
	TextMode textMode;
	Align align;
	GLint alignOffset;

	void UpdateRendertarget();
	void UpdateAttributes();
};