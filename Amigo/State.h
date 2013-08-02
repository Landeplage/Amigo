#pragma once
#include <GL\glew.h>
#include "TinyThread\tinythread.h"
using namespace tthread;

class State
{
public:
	virtual ~State() { }
	virtual void Init() = 0;
	virtual bool AddResources() = 0;
	virtual void Update(GLdouble time) = 0;
	virtual void Draw() = 0;
};