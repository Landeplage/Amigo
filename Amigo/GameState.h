#include "State.h"
#include "Sprite.h"
#include "Player.h"

class GameState : public State
{
public:
	GameState();
	~GameState();
	virtual void Init();
	virtual bool Load();
	virtual void Update(GLdouble time);
	virtual void Draw();

private:
	Sprite sprBackground, sprGround, sprPlayer;
	Player player;
};