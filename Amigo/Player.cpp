#include "Player.h"
#include "Input.h"
#include <GL\glfw.h>

const float acceleration = 0.3f;

Player::Player()
{
	position = Vec2(100, 100);
}

Player::~Player()
{
}

void Player::Update(GLdouble time)
{
	if (Input::GetKey(GLFW_KEY_LEFT))
		{ position.x -= time * acceleration; }
	if (Input::GetKey(GLFW_KEY_RIGHT))
		{ position.x += time * acceleration; }
	if (Input::GetKey(GLFW_KEY_UP))
		{ position.y -= time * acceleration; }
	if (Input::GetKey(GLFW_KEY_DOWN))
		{ position.y += time * acceleration; }
}

void Player::Draw()
{
	sprite->Draw(position.x, position.y);
}

void Player::SetPosition(Vec2 position)
{
	this->position = position;
}

Vec2 Player::GetPosition()
{
	return position;
}

void Player::SetSprite(Sprite* sprite)
{
	this->sprite = sprite;
}