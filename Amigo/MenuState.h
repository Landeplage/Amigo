#include "State.h"
#include "Sprite.h"
#include "Font.h"
#include "MenuSystem.h"
#include "Menu.h"

class MenuState : public State
{
public:
	MenuState();
	~MenuState();

	virtual void Init();
	virtual bool Load();
	virtual void Update(GLdouble time);
	virtual void Draw();

private:
	Sprite sprCursor, sprBackground, sprClouds, sprChequer, sprLogo, sprUI;
	Font FontBold, FontRegular;
	const char* version;
	float cloudScroll, checkerScroll;
	int screenW, screenH;
	bool keyEscPrevious;

	MenuSystem* menuSystem;
	Menu *menuMain, *menuTiny, *menuTiny2;
};