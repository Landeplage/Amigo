#include "Text.h"

const int LINEBREAK_HEIGHT = 20;

Text::Text(MenuSystem* menuSystem, std::string text, Vec2 position, Font* font, GLint menuID)
{
	this->menuSystem = menuSystem;
	this->position = position;
	this->font = font;
	this->menuID = menuID;

	// Init text
	this->text = text;

	// Init alpha
	alpha = 1.0f;

	// Init max width
	maxWidth = 0;

	// Init rendertarget
	rt = new RenderTarget(0, 0);

	// Set the text (must be done after rendertarget has been created)
	SetText(this->text);

	// Init color (must be done after rendertarget has been created)
	SetColor(Color(139, 98, 38));

	// Init shadow's attributes
	shadowColor = Color(255, 255, 255);
	shadowAlpha = 0.6f;
}

void Text::Unload()
{
	// Unload rendertarget
	rt->~RenderTarget();
}

void Text::HandleInput()
{
	// handle input
}

void Text::Update(GLdouble time)
{
	// update
}

void Text::Draw()
{
	// Draw the rendertarget
	rt->Draw(position.x + drawOffset.x, position.y + drawOffset.y - 3, alpha);
}

// Set the text
void Text::SetText(std::string text)
{
	this->text = text;

	// Update attributes
	UpdateAttributes();
}

// Set the color of the text
void Text::SetColor(Color color)
{
	this->color = color;

	// Update attributes
	UpdateAttributes();
}

// Set the alpha to draw the text with
void Text::SetAlpha(GLfloat alpha)
{
	this->alpha = alpha;
}

// Set the max width of the text object before a linebreak occurs
void Text::SetMaxWidth(GLint maxWidth)
{
	this->maxWidth = maxWidth;

	// Update attributes
	UpdateAttributes();
}

// Update the rendertarget
void Text::UpdateRendertarget()
{
	// Set the size
	rt->SetSize(Vec2(size.x, size.y + 3)); // add some pixels so "j", "g", etc, don't get cut off

	// Prepare rendertarget
	rt->Begin();

	if (maxWidth <= 0) // If no width has been specified...
	{
		// Draw highlight (text is drawn slightly downward because "Å" would get cut off)
		font->Draw(0, 4, text, 0.0f, 1.0f, 1.0f, Color(255, 255, 255), 0.6f);

		// Draw text
		font->Draw(0, 3, text, 0.0f, 1.0f, 1.0f, color, 1.0f);
	}
	else // If a linebreak-width has been specified
	{
		// Draw shadow/highlight one pixel below the regular text
		font->DrawLinebreak(0, 4, text, maxWidth, LINEBREAK_HEIGHT, shadowColor, shadowAlpha);

		// Draw text (btw, text is drawn slightly downward to prevent characters like "Å" from getting cut off)
		font->DrawLinebreak(0, 3, text, maxWidth, LINEBREAK_HEIGHT, color, 1.0f);
	}

	// Perform rendering
	rt->End();
}

// Update text object attributes
void Text::UpdateAttributes()
{
	// Update size
	if (maxWidth <= 0) // If no linebreak-width has been specified
	{
		SetSize(Vec2(font->GetWidth(text), font->GetHeight(text) + 1));
	}
	else // If a linebreak-width has been specified
	{
		SetSize(Vec2(font->GetWidth(text, maxWidth), font->GetHeight(text, maxWidth, LINEBREAK_HEIGHT) + 1));
	}

	// Update rendertarget
	UpdateRendertarget();
}

// Set the attributes of the shadow/highlight
void Text::SetShadowAttributes(Color color, GLfloat alpha)
{
	shadowColor = color;
	shadowAlpha = alpha;
}