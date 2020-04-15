#include "stdafx.h"
#include <SceneConfig.h>
SceneConfig::SceneConfig() {}
SceneConfig::~SceneConfig() {}

void SceneConfig::SetScreenWidth(const int & width)
{
	m_screen_width = width;
}

void SceneConfig::SetScreenHeight(const int & height)
{
	m_screen_height = height;
}

void SceneConfig::SetRenderer(SDL_Renderer* pRenderer)
{
	_ASSERT(pRenderer);
	m_pRenderer = pRenderer;
}

void SceneConfig::SetAssetPath(const char* path, const size_t &len)
{
	_ASSERT(path && 0 < len);
	m_asset_path = std::string(path);
}

void SceneConfig::SetDataFile(const char* path, const size_t &len)
{
	_ASSERT(path && 0 < len);
	m_data_file = std::string(path);
}

int SceneConfig::GetScreenWidth() const
{
	return m_screen_width;
}

int SceneConfig::GetScreenHeight() const
{
	return m_screen_height;
}

SDL_Renderer* SceneConfig::GetRenderer() const
{
	return m_pRenderer;
}

std::string SceneConfig::GetAssetPath() const
{
	return m_asset_path;
}

std::string SceneConfig::GetDataFile() const
{
	return m_data_file;
}

void SceneConfig::SetScreenSurface(SDL_Surface* pSurface)
{
	_ASSERT(pSurface);
	m_pScreenSurface = pSurface;
}

SDL_Surface* SceneConfig::GetScreenSurface() const
{
	return m_pScreenSurface;
}