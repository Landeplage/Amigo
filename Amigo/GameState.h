#include "State.h"
#include "Sprite.h"
#include "Player.h"
#include "Font.h"

#include <vector>

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
	Vec2* AddVector(Vec2 position);
	bool isInsideSpline(Vec2 position);

	Sprite sprBackground, sprGround, sprPlayer, sprTest;
	Font font;
	Player player;

	// Curve-variables
	Sprite sprPoint;
	float t;
	Vec2 *last;
	std::vector<Vec2> splinePoints, splineVelocities;
	bool mouseIsInsideSpline;
};