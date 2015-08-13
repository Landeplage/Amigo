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

	// Init textmode
	textMode = TextMode::LINEBREAK;

	// Init alignment
	align = MenuItem::Align::LEFT;
	alignOffset = 0;
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
	rt->Draw(position.x + drawOffset.x + alignOffset, position.y + drawOffset.y - 3, alpha);
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
		font->Draw(Vec2(0, 4), text, 0.0f, Vec2(1.0f, 1.0f), shadowColor, shadowAlpha);

		// Draw text
		font->Draw(Vec2(0, 3), text, 0.0f, Vec2(1.0f, 1.0f), color, 1.0f);
	}
	else // A width has been specified
	{
		switch (textMode)
		{
		case LINEBREAK:
			font->DrawLinebreak(Vec2(0, 4), text, maxWidth, LINEBREAK_HEIGHT, shadowColor, shadowAlpha);
			font->DrawLinebreak(Vec2(0, 3), text, maxWidth, LINEBREAK_HEIGHT, color, 1.0f);
			break;
		case TRUNCATE:
			font->DrawShorten(Vec2(0, 4), text, 0.0f, Vec2(1.0f, 1.0f), shadowColor, shadowAlpha, maxWidth);
			font->DrawShorten(Vec2(0, 3), text, 0.0f, Vec2(1.0f, 1.0f), color, alpha, maxWidth);
			break;
		}
	}

	// Perform rendering
	rt->End();
}

// Update text object attributes
void Text::UpdateAttributes()
{
	// Update size
	if (maxWidth <= 0) // If no maximum-width has been specified
	{
		SetSize(Vec2(font->GetWidth(text), font->GetHeight(text) + 1));
	}
	else // If max width has been specified
	{
		switch (textMode)
		{
		case LINEBREAK:
			SetSize(Vec2(font->GetWidth(text, maxWidth), font->GetHeight(text, maxWidth, LINEBREAK_HEIGHT) + 1));
			break;
		case TRUNCATE:
			SetSize(Vec2(font->GetWidth(font->TruncateString(text, maxWidth)), font->GetHeight(font->TruncateString(text, maxWidth)) + 5));
			break;
		}
	}

	// Update rendertarget
	UpdateRendertarget();
}

// Set the attributes of the shadow/highlight
void Text::SetShadowAttributes(Color color, GLfloat alpha)
{
	shadowColor = color;
	shadowAlpha = alpha;

	// Update attributes
	UpdateAttributes();
}

// Set how the text handles strings that are wider than maxWidth
void Text::SetTextMode(TextMode textMode)
{
	this->textMode = textMode;

	// Update attributes
	UpdateAttributes();
}

// Set how the text aligns
void Text::SetAlign(Align align)
{
	this->align = align;

	// Find out the width of the text
	GLint tmpWidth = 0;
	switch (textMode)
	{
	case TextMode::LINEBREAK: tmpWidth = font->GetWidth(text, maxWidth); break;
	case TextMode::TRUNCATE: tmpWidth = font->GetWidth(font->TruncateString(text, maxWidth)); break;
	}

	// Set alignment offset based on width
	switch (align)
	{
	case Align::LEFT: break;
	case Align::RIGHT: alignOffset = -tmpWidth; break;
	case Align::CENTER: alignOffset = -(tmpWidth / 2); break;
	}
}