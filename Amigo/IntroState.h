#include "State.h"
#include "Sprite.h"

class IntroState : public State
{
public:
	IntroState();
	~IntroState();
	virtual void Init();
	virtual bool Load();
	virtual void Update(GLdouble time);
	virtual void Draw();

private:
	bool finishedLoading;
	Sprite sprLogo;
	int timerWait;
};