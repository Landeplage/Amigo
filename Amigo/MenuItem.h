#pragma once
#include <GL\glew.h>
#include <vector>
#include "Vec2.h"
#include "Sprite.h"
#include <functional>

class MenuSystem;

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

	virtual void HandleInput() = 0;
	virtual void Update(GLdouble time) = 0;
	virtual void Draw() = 0;
	virtual void Unload() = 0;

	Vec2 GetPosition();
	Vec2 GetSize();
	Vec2 GetOrigin();
	Vec2 GetDrawOffset();
	std::vector<MenuItem*> GetChildren();

	virtual void SetPosition(Vec2 position);
	virtual void SetSize(Vec2 size);
	virtual void SetOrigin(Vec2 origin);
	virtual void SetDrawOffset(Vec2 drawOffset);

	void SetPositionCallback(std::function<void()> callback);
	void SetSizeCallback(std::function<void()> callback);
	void SetOriginCallback(std::function<void()> callback);
	void SetDrawOffsetCallback(std::function<void()> callback);

	bool active, visible;
	int menuID;

protected:
	Vec2 size, position, origin, drawOffset;
	std::vector<MenuItem*> children;
	Sprite *sprite;
	MenuSystem *menuSystem;

	// Callback-functions
	std::function<void()>
		setPositionCallback,
		setSizeCallback,
		setOriginCallback,
		setDrawOffsetCallback;
};