#include "State.h"
#include "RenderTarget.h"
#include "Sprite.h"

class TestState : public State
{
public:
	TestState();
	~TestState();
	virtual void Init();
	virtual bool Load();
	virtual void Update(GLdouble time);
	virtual void Draw();

private:
	Sprite sprBackground, sprTest;
	RenderTarget *renderTargetTest;
};