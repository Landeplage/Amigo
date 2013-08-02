#include "ResourceManager.h"

ResourceManager* ResourceManager::instance = 0;

ResourceManager::ResourceManager()
{
	font = new Font();
	isLoading = false;
	simulatedLoadingTime = 0.0; // debug
	simulatedLoadingCount = 0;
}

void ResourceManager::Load()
{
	font->LoadFont("res\\font\\robotob.ttf", 12);
}

void ResourceManager::Update(GLdouble time)
{
	// Load in new resources
	if (simulatedLoadingTime <= 0.0)
	{
		if (simulatedLoadingCount == (fontResources.size() + spriteResources.size()) || fontResources.size() == 0)
		{
			isLoading = false;
			simulatedLoadingCount = 0;
		}
		else
		{
			// Load resources
			if (simulatedLoadingCount < fontResources.size())
			{
				fontResources[simulatedLoadingCount]->font->LoadFont(
							fontResources[simulatedLoadingCount]->path,
							fontResources[simulatedLoadingCount]->fontSize); // fonts
			}
			else if (simulatedLoadingCount < fontResources.size() + spriteResources.size())
			{
				spriteResources[simulatedLoadingCount - fontResources.size()]->sprite->LoadTexture(
					spriteResources[simulatedLoadingCount - fontResources.size()]->path); // sprites
			}

			simulatedLoadingTime = 100.0;
			simulatedLoadingCount ++;
		}
	}
	else
	{
		simulatedLoadingTime -= time;
	}
}

void ResourceManager::Draw()
{
	font->Draw(10, 10, "Loading...");
	const char* str;
	float a;

	for(int i = 0; i < fontResources.size() + spriteResources.size(); i ++)
	{
		if (simulatedLoadingCount > i)
			a = 1.0f;
		else
			a = 0.5f;
		if (i < fontResources.size())
		{
			str = fontResources[i]->path;
			
		}
		else if (i < fontResources.size() + spriteResources.size())
		{
			str = spriteResources[i - fontResources.size()]->path;
		}
		font->Draw(20, 30 + i * 18, str, 0.0f, 1.0f, 1.0f, Color(255, 255, 255), a); // fonts
	}
}

void ResourceManager::Cleanup()
{
	font->~Font();
}

void ResourceManager::StartLoading()
{
	isLoading = true;
}

// Unload all currently loaded resources
void ResourceManager::UnloadAll()
{
	for(int i = 0; i < fontResources.size(); i ++)
	{ fontResources[i]->font->~Font(); }
	for(int i = 0; i < spriteResources.size(); i ++)
	{ spriteResources[i]->sprite->~Sprite(); }
	fontResources.clear();
	spriteResources.clear();
}

bool ResourceManager::IsLoading()
{
	return isLoading;
}

// Add a font to be loaded
void ResourceManager::AddFont(Font* font, const char* path, int fontSize)
{
	fontResources.push_back(new FontResource(font, path, fontSize));
}

// Add a sprite to be loaded
void ResourceManager::AddSprite(Sprite *sprite, const char* path)
{
	spriteResources.push_back(new SpriteResource(sprite, path));
}

// Return the instance of ResourceManager (singleton)
ResourceManager* ResourceManager::GetInstance()
{
	if (!instance)
	{
		instance = new ResourceManager();
		instance->Load();
	}
	return instance;
}