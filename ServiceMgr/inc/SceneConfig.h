#pragma once
#include <SDL.h>
#include <base_define.h>
#include <type_define.h>

class SceneConfig : public SingletonHolder<SceneConfig>
{
protected:
	friend class SingletonHolder<SceneConfig>;
	int m_screen_width = -1;
	int m_screen_height = -1;

	SDL_Renderer* m_pRenderer = nullptr;
	std::string m_asset_path = "";
	std::string m_data_file = "";

	SDL_Surface* m_pScreenSurface = nullptr;

	SceneConfig();
public:
	void SetScreenWidth(const int &width);
	void SetScreenHeight(const int &height);
	void SetRenderer(SDL_Renderer* pRenderer);
	void SetAssetPath(const char* path, const size_t &len);
	void SetDataFile(const char* path, const size_t &len);

	int GetScreenWidth() const;
	int GetScreenHeight() const;
	SDL_Renderer* GetRenderer() const;
	std::string GetAssetPath() const;
	std::string GetDataFile() const;

	void SceneConfig::SetScreenSurface(SDL_Surface* pSurface);
	SDL_Surface* GetScreenSurface() const;

	~SceneConfig();

};
