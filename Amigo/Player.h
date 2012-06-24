#pragma once
#include <GL\glew.h>
#include "Vec2.h"
#include "Sprite.h"

class Player
{
public:
	Player();
	~Player();

	void Update(GLdouble time);
	void Draw();

	void SetPosition(Vec2 position);
	Vec2 GetPosition();
	void SetSprite(Sprite* sprite);

private:
	Vec2 position;
	Sprite* sprite;
};