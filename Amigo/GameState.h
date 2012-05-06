#pragma once
#include "TinyThread\tinythread.h"
using namespace tthread;

class GameState
{
public:
	virtual ~GameState() { }
	virtual void Init() = 0;
	virtual bool Load() = 0;
	virtual void Update(GLdouble time) = 0;
	virtual void Draw() = 0;
};