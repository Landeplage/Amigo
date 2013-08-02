#pragma once
#include <GL\glew.h>
#include "Font.h"
#include "Sprite.h"
#include <vector>

// This is a singleton class!
class ResourceManager
{
public:
	ResourceManager();

	static ResourceManager* instance;
	static ResourceManager* GetInstance();

	void Load();
	void Update(GLdouble time);
	void Draw();
	void Cleanup();
	void StartLoading();
	void UnloadAll();
	bool IsLoading();

	void AddFont(Font* font, const char* path, int fontSize); // adds a font to be loaded
	void AddSprite(Sprite* sprite, const char* path); // adds a sprite to be loaded
	//void AddSound(const char* path); // adds a sound to be loaded

private:
	bool isLoading;
	Font *font;
	GLdouble simulatedLoadingTime;
	GLint simulatedLoadingCount;

	// font
	struct FontResource
	{
		FontResource(Font* font, const char* path, int fontSize)
		{
			this->font = font;
			this->path = path;
			this->fontSize = fontSize;
		}

		Font* font;
		const char* path;
		int fontSize;
	};

	// sprite
	struct SpriteResource
	{
		SpriteResource(Sprite* sprite, const char* path)
		{
			this->sprite = sprite;
			this->path = path;
		}

		Sprite* sprite;
		const char* path;
	};

	std::vector<FontResource*> fontResources;
	std::vector<SpriteResource*> spriteResources;
};