#include "State.h"
#include "Sprite.h"
#include "Font.h"
#include "RenderTarget.h"

class BlendtestState : public State
{
public:
	BlendtestState();
	~BlendtestState();

	void Init();
	void AddResources();
	void HandleInput();
	void Update(GLdouble time);
	void Draw();

private:
	Sprite *sprBg, *sprTest;
	Font *font;
	RenderTarget *rt, *rt2;
};