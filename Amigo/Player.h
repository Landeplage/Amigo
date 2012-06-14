#pragma once
#include <GL\glew.h>
#include "Point.h"
#include "Sprite.h"

class Player
{
public:
	Player();
	~Player();

	void Update(GLdouble time);
	void Draw();

	void SetPosition(Point position);
	Point GetPosition();
	void SetSprite(Sprite* sprite);

private:
	Point position;
	Sprite* sprite;
};