#include "ContentBox.h"
#include "MenuSystem.h"
#include "Input.h"
#include "Helper.h"

// Scrollerbar constants
const int
	SB_WIDTH = 14,
	SB_BMINHEIGHT = 30;

ContentBox::ContentBox(MenuSystem* menuSystem, std::string title, GLint x, GLint y, GLint w, GLint h, GLint menuID)
{
	this->menuSystem = menuSystem;
	this->position = Vec2(x, y);
	this->size = Vec2(w, h);
	this->menuID = menuID;

box = new Box(menuSystem, title, x, y, w, h, menuID);
scrollButtonVer = new Button(menuSystem, "", Vec2(position.x + size.x - SB_WIDTH, position.y), Vec2(SB_WIDTH, SB_BMINHEIGHT), MenuItem::Align::CENTER, menuID, "", []() {});
scrollButtonVer->active = false;
scrollButtonHor = new Button(menuSystem, "", Vec2(position.x, position.y + size.y - SB_WIDTH), Vec2(SB_BMINHEIGHT, SB_WIDTH), MenuItem::Align::CENTER, menuID, "", []() {});
scrollButtonHor->active = false;
scrollButtonMouseOffset = Vec2();
scroll = Vec2(0.0f, 0.0f);
scrollContentSize.y = 0.0f;

// Add scrollbutton to list of children
children.push_back(scrollButtonHor);
children.push_back(scrollButtonVer);

rt = new RenderTarget(0, 0);
}

void ContentBox::HandleInput()
{
	box->HandleInput();
	scrollButtonHor->HandleInput();
	scrollButtonVer->HandleInput();
	HandleHorizontalScrollButton();
	HandleVerticalScrollButton();

	GLint sbOffW = 0, sbOffH = 0;
	if (scrollButtonVer->active)
		sbOffW = SB_WIDTH;
	if (scrollButtonHor->active)
		sbOffH = SB_WIDTH;

	// Set flag to make sure items are handled only once
	bool activeItemsHaveBeenHandled = false;

	Vec2 mouse = Input::getMousePos() - origin;
	if (mouse.y > position.y && mouse.y < position.y + size.y - sbOffH && mouse.x > position.x && mouse.x < position.x + size.x - sbOffW)
	{
		if (scrollContentSize.y > size.y && !(menuSystem->IsDropdownActive()))
		{
			// Wheel-scrolling
			menuSystem->SetCurrentScrollboxFocus(this);
			mouseWheelScroll = Input::getMouseWheel();
		}

		// Handle input of all active items
		if (!activeItemsHaveBeenHandled)
		{
			for (int i = 0; i < activeItems.size(); i++)
			{
				activeItems[i]->HandleInput();
			}
			activeItemsHaveBeenHandled = true;
		}
	}
	else
	{
		// Reset scrollbox focus
		if (menuSystem->GetCurrentScrollboxFocus() == this)
			menuSystem->ResetCurrentScrollboxFocus();

		// Reset focus of items, unless they are currently being used
		if (menuSystem->GetFocus() != NULL)
		{
			MenuItem *focus = menuSystem->GetFocus();
			bool checked = false;
			checked = CheckChildrenFocus(activeItems, focus);

			if (Input::getMouseLeft())
			{
				if (checked && !activeItemsHaveBeenHandled)
				{
					// Handle input of all active items (basically because sliders' buttons will stop updating if user grabs it and then the cursor leaves the contentbox)
					for (int i = 0; i < activeItems.size(); i++)
					{
						activeItems[i]->HandleInput();
					}
					activeItemsHaveBeenHandled = true;
				}
			}
			else
			{
				// Reset focus if focus is on any item in content, or subitems within content, unless some item has taken control of focus (f.ex. a dropdown)
				if (checked && !(menuSystem->IsDropdownActive()))
				{
					menuSystem->ResetFocus();
					menuSystem->ResetTooltip();
				}
			}
		}

		// If a dropdown within the contentbox has been activated, handle update on it
		if (menuSystem->IsDropdownActive())
		{
			bool checked = false;
			checked = CheckChildrenFocus(activeItems, menuSystem->GetFocus());

			if (checked)
			{
				menuSystem->GetFocus()->HandleInput();
			}
		}

		// If an inputField within the contentbox has been activated, handle update on all items (in case it's inside the contentbox somewhere
		InputField *inputField;
		inputField = menuSystem->GetActiveInputField();
		if (inputField != NULL && !activeItemsHaveBeenHandled)
		{
			for (int i = 0; i < activeItems.size(); i++)
			{
				activeItems[i]->HandleInput();
			}
			activeItemsHaveBeenHandled = true;
			/*
			bool checked = false;
			checked = CheckChildrenFocus(activeItems, inputField);

			if (checked)
			{
				inputField->HandleInput();
			}*/
		}
	}
}

// Returns true if any item or any of their children in this contentbox is currently focused
bool ContentBox::CheckChildrenFocus(std::vector<MenuItem*> items, MenuItem* focus)
{
	if (focus == NULL)
		return false;

	for(int i = 0; i < items.size(); i ++)
	{
		if (focus == items[i])
		{
			return true;
		}
		else
		{
			if (CheckChildrenFocus(items[i]->GetChildren(), focus))
				return true;
		}
	}
	
	return false;
}

void ContentBox::Update(GLdouble time)
{
	if (menuSystem->GetCurrentScrollboxFocus() == this)
	{
		if (mouseWheelScroll != 0.0f)
			SetScrollY(scroll.y - mouseWheelScroll * (25.0f / (scrollContentSize.y - scrollButtonVer->GetSize().y)));
	}

	box->Update(time);
	scrollButtonHor->Update(time);
	scrollButtonVer->Update(time);

	for(int i = 0; i < activeItems.size(); i ++)
	{
		activeItems[i]->Update(time);
	}

	// Render items to rendertarget
	Render();
}

void ContentBox::Draw()
{
	// Draw rendertarget (truncate edges to fit neatly in box)
	if (activeItems.size() > 0)
		rt->Draw(position.x + 1 + drawOffset.x, position.y + 1 + drawOffset.y, 0.0f, 1.0f, 1.0f, Color(255, 255, 255), 1.0f, 1, 1, rt->GetSize().x - 2, rt->GetSize().y - 2);

	// Draw the box outline
	box->Draw();

	// Horizontal scroller
	if (scrollButtonHor->active)
	{
		menuSystem->GetSpriteUI()->Draw(Vec2(position.x + drawOffset.x + 1, scrollButtonHor->GetPosition().y + drawOffset.y),  0.0f, Vec2(size.x - 2, 1.0f), Color(255, 255, 255), 1.0f, 0, 52, 1, 1); // line
		scrollButtonHor->Draw();
		menuSystem->GetSpriteUI()->Draw(Vec2(scrollButtonHor->GetPosition().x + drawOffset.x + scrollButtonHor->GetSize().x / 2 - 5, scrollButtonHor->GetPosition().y + drawOffset.y + (SB_WIDTH - 8) / 2 + 8), 270.0f, Vec2(8.0f, 1.0f), 1.0f, 8, 31, 1, 10); // lines on scrollerbutton
	}

	// Vertical scroller
	if (scrollButtonVer->active)
	{
		menuSystem->GetSpriteUI()->Draw(Vec2(scrollButtonVer->GetPosition().x + drawOffset.x, position.y + drawOffset.y + 1), 0.0f, Vec2(1.0f, size.y - 2), Color(255, 255, 255), 1.0f, 0, 52, 1, 1); // line
		scrollButtonVer->Draw();
		menuSystem->GetSpriteUI()->Draw(Vec2(scrollButtonVer->GetPosition().x + drawOffset.x + (SB_WIDTH - 8) / 2, scrollButtonVer->GetPosition().y + drawOffset.y + scrollButtonVer->GetSize().y / 2 - 5), 0.0f, Vec2(8.0f, 1.0f), 1.0f, 8, 31, 1, 10); // lines on scrollerbutton
	}
}

// Render the items onto the rendertarget
void ContentBox::Render()
{
	rt->Begin();

	// Uncomment to make size of rendertarget visible
	//menuSystem->GetSpriteUI()->Draw(0, 0, 0.0f, 1280.0f, 720.0f, Color(255, 255, 255), 0.5f, 5, 5, 1, 1);

	for(int i = 0; i < activeItems.size(); i ++)
	{
		activeItems[i]->Draw();
	}

	rt->End();
}

void ContentBox::Unload()
{
	for(int i = 0; i < items.size(); i ++)
	{
		items[i]->Unload();
	}
}

// Add an item to the contentbox (automatic positioning)
MenuItem* ContentBox::AddItem(MenuItem* item)
{
	GLint
		y = 0,
		i = items.size();

	if (i > 0)
			y = items[i - 1]->GetPosition().y + items[i - 1]->GetSize().y;

	AddItem(item, item->GetPosition() + Vec2(0 , y), item->GetSize());

	return item;
}

// Add an item to the contentbox (positioned and sized manually, relative to contentbox)
MenuItem* ContentBox::AddItem(MenuItem* item, Vec2 position, Vec2 size)
{
	// Add new item to vector
	items.push_back(item);
	
	// Set some attributes
	item->SetPosition(position);
	item->SetSize(size);
	item->SetOrigin(this->position);

	// Set callback functions for when item changes position or size
	item->SetPositionCallback([=]()
	{
		RefreshContentBox();
	});

	item->SetSizeCallback([=]()
	{
		RefreshContentBox();
	});

	// Update the scroll content size
	UpdateScrollContentSize();

	// Enable scroller-button if need be, and autosize it
	UpdateHorizontalScrollButtonAttributes();
	UpdateVerticalScrollButtonAttributes();

	// Repeat scrollbutton attribute updates, in case either changed state
	UpdateHorizontalScrollButtonAttributes();
	UpdateVerticalScrollButtonAttributes();

	// Make sure scrollers are moved to their places, and items are updated
	SetScrollX(scroll.x);
	SetScrollY(scroll.y);

	// Resize rendertarget
	ResizeRendertarget();

	// Render items to rendertarget
	Render();

	// Add item to list of children
	children.push_back(item);

	// Return pointer to item added
	return item;
}

// Remove an item from the contentbox
void ContentBox::RemoveItem(MenuItem* item)
{
	// Loop through items and find item index
	GLint index = -1;
	for(int i = 0; i < items.size(); i ++)
	{
		if (items[i] == item)
		{
			index = i;
			break;
		}
	}

	// If index is valid...
	if (index > -1)
	{
		// Move any items below the one about to be removed
		GLint
			itemY = item->GetPosition().y,
			itemH = item->GetSize().y;
		for(int i = 0; i < items.size(); i ++)
		{
			if (items[i]->GetPosition().y > itemY + itemH)
				items[i]->SetPosition(items[i]->GetPosition() - Vec2(0, itemH));
		}

		// Remove the item from list of children
		for(int i = 0; i < children.size(); i ++)
		{
			if (children[i] = items[index])
			{
				children.erase(children.begin() + i);
				break;
			}
		}

		//...remove the item from item list.
		items.erase(items.begin() + index);

		// Update the scroll content height
		UpdateScrollContentSize();

		// Make sure the scroller updates properly
		SetScrollX(scroll.x);
		SetScrollY(scroll.y);
	}
}

void ContentBox::HandleHorizontalScrollButton()
{
	if (menuSystem->GetFocus() == scrollButtonHor && scrollButtonHor->GetState() == 2)
	{
		Vec2 mouse = Input::getMousePos() - origin;

		if (Input::getMouseLeftPressed())
		{
			scrollButtonMouseOffset = mouse - scrollButtonHor->GetPosition();
		}

		if (Input::getMouseLeft())
		{
			scrollButtonHor->SetPosition(Vec2(mouse.x - scrollButtonMouseOffset.x, scrollButtonHor->GetPosition().y)); // move button
			//Input::setMousePos(Vec2(Input::getMousePos().x, scrollButtonHor->GetPosition().y + origin.y + scrollButtonMouseOffset.y)); // lock mouse vertically

			LimitHorizontalScrollButton();

			GLint sbOff = 0;
			if (scrollButtonVer->active)
			{
				sbOff = SB_WIDTH;
			}

			SetScrollX((scrollButtonHor->GetPosition().x - position.x) / ((size.x - sbOff) - scrollButtonHor->GetSize().x));
		}
	}
}

void ContentBox::HandleVerticalScrollButton()
{
	if (menuSystem->GetFocus() == scrollButtonVer && scrollButtonVer->GetState() == 2)
	{
		Vec2 mouse = Input::getMousePos() - origin;

		if (Input::getMouseLeftPressed())
		{
			scrollButtonMouseOffset = mouse - scrollButtonVer->GetPosition();
		}

		if (Input::getMouseLeft())
		{
			GLfloat yy;
			yy = mouse.y - scrollButtonMouseOffset.y;
			scrollButtonVer->SetPosition(Vec2(scrollButtonVer->GetPosition().x, yy)); // move button
			//Input::setMousePos(Vec2(scrollButtonVer->GetPosition().x + origin.x + scrollButtonMouseOffset.x, Input::getMousePos().y)); // lock mouse horizontally

			LimitVerticalScrollButton();

			GLint sbOff = 0;
			if (scrollButtonHor->active)
			{
				sbOff = SB_WIDTH;
			}

			SetScrollY((scrollButtonVer->GetPosition().y - position.y) / ((size.y - sbOff) - scrollButtonVer->GetSize().y));
		}
	}
}

void ContentBox::LimitHorizontalScrollButton()
{
	GLint sbOff = 0;
	if (scrollButtonVer->active)
	{
		sbOff = SB_WIDTH;
	}

	// Make sure button stays inside scrollbar
	if (scrollButtonHor->GetPosition().x < position.x) // limit left
	{
		scrollButtonHor->SetPosition(Vec2(position.x, scrollButtonHor->GetPosition().y));
	}
	else if (scrollButtonHor->GetPosition().x + scrollButtonHor->GetSize().x > position.x + (size.x - sbOff)) // limit right
	{
		scrollButtonHor->SetPosition(Vec2(position.x + (size.x - sbOff) - scrollButtonHor->GetSize().x, scrollButtonHor->GetPosition().y));
	}
}

void ContentBox::LimitVerticalScrollButton()
{
	GLint sbOff = 0;
	if (scrollButtonHor->active)
	{
		sbOff = SB_WIDTH;
	}

	// Make sure button stays inside scrollbar
	if (scrollButtonVer->GetPosition().y < position.y) // limit top
	{
		scrollButtonVer->SetPosition(Vec2(scrollButtonVer->GetPosition().x, position.y));
	}
	else if (scrollButtonVer->GetPosition().y + scrollButtonVer->GetSize().y > position.y + (size.y - sbOff)) // limit bottom
	{
		scrollButtonVer->SetPosition(Vec2(scrollButtonVer->GetPosition().x, position.y + (size.y - sbOff) - scrollButtonVer->GetSize().y));
	}
}

void ContentBox::UpdateHorizontalScrollButtonAttributes()
{
	GLint sbOff = 0;
	if (scrollButtonVer->active)
		sbOff = SB_WIDTH;

	// Enable scroller-button if need be, and autosize it
	if (scrollContentSize.x > this->size.x - sbOff)
	{
		scrollButtonHor->active = true;
		GLint buttonWidth = SB_BMINHEIGHT;
		if ((this->size.x / (scrollContentSize.x + sbOff)) * (this->size.x - sbOff) > SB_BMINHEIGHT)
			buttonWidth = (this->size.x / (scrollContentSize.x + sbOff)) * (this->size.x - sbOff);
		scrollButtonHor->SetSize(Vec2(buttonWidth, scrollButtonHor->GetSize().y));
	}
	else
	{
		scrollButtonHor->active = false;
	}
}

void ContentBox::UpdateVerticalScrollButtonAttributes()
{
	GLint sbOff = 0;
	if (scrollButtonHor->active)
		sbOff = SB_WIDTH;

	// Enable scroller-button if need be, and autosize it
	if (scrollContentSize.y > this->size.y - sbOff)
	{
		scrollButtonVer->active = true;
		GLint buttonHeight = SB_BMINHEIGHT;
		if ((this->size.y / (scrollContentSize.y + sbOff)) * (this->size.y - sbOff) > SB_BMINHEIGHT)
			buttonHeight = (this->size.y / (scrollContentSize.y + sbOff)) * (this->size.y - sbOff);
		scrollButtonVer->SetSize(Vec2(scrollButtonVer->GetSize().x, buttonHeight));
	}
	else
	{
		scrollButtonVer->active = false;
	}
}

void ContentBox::UpdateScrollContentSize()
{
	// Find total height of the content/items
	GLint
		tmph = 0,
		tmpw = 0,
		h = 0,
		w = 0;
	for(int i = 0; i < items.size(); i ++)
	{
		tmpw = items[i]->GetPosition().x + items[i]->GetSize().x + 10; // the + 10 is to add some extra headroom
		if (tmpw > w)
			w = tmpw;
		tmph = items[i]->GetPosition().y + items[i]->GetSize().y + 10;
		if (tmph > h)
			h = tmph;
	}

	// Apply new width/height
	scrollContentSize = Vec2(w, h);
}

void ContentBox::SetScrollX(GLfloat scroll)
{
	// Clamp scroll
	if (scroll < 0.0f)
		scroll = 0.0f;
	else
		if (scroll > 1.0f)
			scroll = 1.0f;

	// Apply new scroll-value
	this->scroll.x = scroll;

	GLint sbOff = 0;
	if (scrollButtonVer->active)
	{
		sbOff = SB_WIDTH;
	}

	// Move button
	scrollButtonHor->SetPosition(Vec2(position.x + scroll * ((size.x - sbOff) - scrollButtonHor->GetSize().x), scrollButtonHor->GetPosition().y));

	// Limit button
	LimitHorizontalScrollButton();

	// Update the list of active items
	UpdateActiveItemsList();

	// Update origin/drawoffset of items
	UpdateItemAttributes();
}

void ContentBox::SetScrollY(GLfloat scroll)
{
	// Clamp scroll
	if (scroll < 0.0f)
		scroll = 0.0f;
	else
		if (scroll > 1.0f)
			scroll = 1.0f;

	// Apply new scroll-value
	this->scroll.y = scroll;

	GLint sbOff = 0;
	if (scrollButtonHor->active)
	{
		sbOff = SB_WIDTH;
	}

	// Move button
	scrollButtonVer->SetPosition(Vec2(scrollButtonVer->GetPosition().x, position.y + scroll * ((size.y - sbOff) - scrollButtonVer->GetSize().y)));

	// Limit button
	LimitVerticalScrollButton();

	// Update the list of active items
	UpdateActiveItemsList();

	// Update origin/drawoffset of items
	UpdateItemAttributes();
}

// Update drawoffset/origin of any visible items
void ContentBox::UpdateItemAttributes()
{
	GLint sbW = 0, sbH = 0;
	if (scrollButtonHor->active)
		sbH = SB_WIDTH;
	if (scrollButtonVer->active)
		sbW = SB_WIDTH;

	Vec2 scrollOffset = Vec2(0.0f, 0.0f);
	if (scrollContentSize.x > size.x - sbW)
		scrollOffset.x = (int)(scroll.x * (scrollContentSize.x + sbW - size.x));
	if (scrollContentSize.y > size.y - sbH)
		scrollOffset.y = (int)(scroll.y * (scrollContentSize.y + sbH - size.y));
	for(int i = 0; i < activeItems.size(); i ++)
	{
		activeItems[i]->SetDrawOffset(Vec2(-scrollOffset.x, -scrollOffset.y));
		activeItems[i]->SetOrigin(position + origin - scrollOffset);
	}
}

// Clear all items and add currently visible items to the "bucket"
void ContentBox::UpdateActiveItemsList()
{
	// Wipe list
	activeItems.clear();

	// Add all currently visible items
	GLint
		minY = scroll.y * (scrollContentSize.y - size.y),
		maxY = minY + size.y + 20;
	for(int i = 0; i < items.size(); i ++)
	{
		if (items[i]->GetPosition().y + items[i]->GetSize().y > minY && items[i]->GetPosition().y < maxY)
			activeItems.push_back(items[i]);
	}
}

void ContentBox::ResizeRendertarget()
{
	// Recalculate content size
	UpdateScrollContentSize();

	// Limit the rt to contentbox edges
	GLint
		w = scrollContentSize.x,
		h = scrollContentSize.y,
		sbW = 0,
		sbH = 0;

	if (scrollButtonHor->active)
		sbH = SB_WIDTH;
	if (scrollButtonVer->active)
		sbW = SB_WIDTH;

	if (w > size.x - sbW)
		w = size.x - sbW;
	if (h > size.y - sbH)
		h = size.y - sbH;

	// Apply width/height
	rt->SetSize(Vec2(w, h));
}

void ContentBox::SetPosition(Vec2 position)
{
	MenuItem::SetPosition(position);
	box->SetPosition(position);
	scrollButtonVer->SetPosition(position + Vec2(size.x - SB_WIDTH, 0));
	SetScrollX(scroll.x); // <-- this ensures that the scrollbutton moves into place
	SetScrollY(scroll.y); // <-- this ensures that the scrollbutton moves into place

	// Make sure all items are moved with the contentbox
	UpdateItemAttributes();
}

void ContentBox::SetSize(Vec2 size)
{
	MenuItem::SetSize(size);
	box->SetSize(size);

	// Move scrollbuttons to default positions according to new size
	scrollButtonHor->SetPosition(Vec2(position.x, position.y + size.y - SB_WIDTH));
	scrollButtonVer->SetPosition(Vec2(position.x + size.x - SB_WIDTH, position.y));

	//Hide/show scrollbuttons, and resize them
	UpdateHorizontalScrollButtonAttributes();
	UpdateVerticalScrollButtonAttributes();

	// This moves the scrollbutton to where it should be
	SetScrollX(scroll.x);
	SetScrollY(scroll.y);

	// Update active items list
	UpdateActiveItemsList();

	// Resize rendertarget
	ResizeRendertarget();
}

void ContentBox::SetOrigin(Vec2 origin)
{
	MenuItem::SetOrigin(origin);
	box->SetOrigin(origin);
	scrollButtonHor->SetOrigin(origin);
	scrollButtonVer->SetOrigin(origin);
	UpdateItemAttributes();
}

void ContentBox::SetDrawOffset(Vec2 drawOffset)
{
	MenuItem::SetDrawOffset(drawOffset);
	box->SetDrawOffset(drawOffset);
	scrollButtonHor->SetDrawOffset(drawOffset);
	scrollButtonVer->SetDrawOffset(drawOffset);
	UpdateItemAttributes();
}

// Do a complete refresh of the contentbox
void ContentBox::RefreshContentBox()
{
	// Update the scroll content size
	UpdateScrollContentSize();

	// Enable scroller-button if need be, and autosize it
	UpdateHorizontalScrollButtonAttributes();
	UpdateVerticalScrollButtonAttributes();

	// Repeat scrollbutton attribute updates, in case either changed state
	UpdateHorizontalScrollButtonAttributes();
	UpdateVerticalScrollButtonAttributes();

	// Make sure scrollers are moved to their places, and items are updated
	SetScrollX(scroll.x);
	SetScrollY(scroll.y);

	// Resize rendertarget
	ResizeRendertarget();

	// Render items to rendertarget
	Render();
}