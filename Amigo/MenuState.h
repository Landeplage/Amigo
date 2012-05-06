#include "GameState.h"
#include "Sprite.h"
#include "Font.h"

class MenuState : public GameState
{
public:
	MenuState();
	~MenuState();

	virtual void Init();
	virtual bool Load();
	virtual void Update(GLdouble time);
	virtual void Draw();

private:
	bool finishedLoading;
	Sprite sprCursor, sprBackground, sprTest, sprUI;
	GLfloat rot;
	Font FontBold, FontRegular;
	const char* version;
};