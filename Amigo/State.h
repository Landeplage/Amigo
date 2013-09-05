#pragma once
#include <GL\glew.h>
#include "TinyThread\tinythread.h"
using namespace tthread;

class State
{
public:
	virtual ~State() { }
	virtual void Init() = 0;
	virtual void AddResources() = 0;
	virtual void HandleInput() = 0;
	virtual void Update(GLdouble time) = 0;
	virtual void Draw() = 0;
};