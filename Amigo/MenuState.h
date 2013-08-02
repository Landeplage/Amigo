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
	virtual bool AddResources();
	virtual void Update(GLdouble time);
	virtual void Draw();

	void CreateMenu();

private:
	Sprite *sprCursor, *sprBack1, *sprBack2, *sprBack3, *sprChequer, *sprLogo, *sprLogoSmall, *sprUI;
	Font *fontBold, *fontRegular, *fontBig;

	const char* version;
	float checkerScroll;
	int screenW, screenH;
	bool keyEscPrevious;

	MenuSystem* menuSystem;
	Menu *menuMain, *menuTiny;
};