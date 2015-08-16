#pragma once
#include "MenuItem.h"
#include "Box.h"
#include "Button.h"
#include "RenderTarget.h"
#include <vector>

class ContentBox : public MenuItem
{
public:
	ContentBox(MenuSystem* menuSystem, std::string title, GLint x, GLint y, GLint w, GLint h, GLint menuID);

	void HandleInput();
	void Update(GLdouble time);
	virtual void Draw();
	void Unload();

	MenuItem* AddItem(MenuItem *item);
	MenuItem* AddItem(MenuItem *item, Vec2 position, Vec2 size);
	void RemoveItem(MenuItem *item);

	void SetScrollX(GLfloat scroll);
	void SetScrollY(GLfloat scroll);

	void SetPosition(Vec2 position);
	void SetSize(Vec2 size);
	void SetOrigin(Vec2 origin);
	void SetDrawOffset(Vec2 drawOffset);

	void SetPadding(GLint padding);
	void SetHorizontalPadding(GLint horPadding);
	void SetVerticalPadding(GLint verPadding);

	void UpdateItemAttributes();

	enum ResizeMode
	{
		NONE, // no automatic resizing when adding items
		RESIZE_WIDTH // auto-adjust width on items
	};
	void SetResizeMode(ResizeMode resizeMode);

private:
	void Render();
	void UpdateActiveItemsList();
	bool CheckChildrenFocus(std::vector<MenuItem*> items, MenuItem* focus);
	bool CheckChildrenFocus(MenuItem* item, MenuItem* focus);
	void ResizeRendertarget();
	void UpdateScrollContentSize();
	void RefreshContentBox();
	void ResizeItems();

	void HandleHorizontalScrollButton();
	void HandleVerticalScrollButton();
	void LimitHorizontalScrollButton();
	void LimitVerticalScrollButton();
	void UpdateHorizontalScrollButtonAttributes();
	void UpdateVerticalScrollButtonAttributes();

	Box *box;
	RenderTarget *rt;
	std::vector<MenuItem*> items;
	std::vector<MenuItem*> activeItems;

	// Scroller members
	Button *scrollButtonHor, *scrollButtonVer;
	Vec2 scrollButtonMouseOffset;
	GLfloat mouseWheelScroll;
	Vec2 scroll, scrollContentSize;

	// Resize-mode members
	ResizeMode resizeMode;

	// Padding members
	GLint verPadding, horPadding;
};