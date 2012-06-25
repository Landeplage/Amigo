#pragma once
#include <GL\glew.h>
#include "Vec2.h"
#include "Sprite.h"

class MenuItem
{
public:
	MenuItem();

	enum Align
	{
		TOP,
		LEFT,
		CENTER,
		RIGHT,
		BOTTOM
	};

	virtual void Update(GLdouble time) = 0;
	virtual void Draw() = 0;
	virtual void Unload() = 0;

	Vec2 GetSize();
	Vec2 GetPosition();

	void SetSize(GLint width, GLint height);
	void SetPosition(GLint x, GLint y);

	bool active, visible;
	int menuID;

protected:
	Vec2 size, position, menuOffset;
	Sprite *sprite;
};