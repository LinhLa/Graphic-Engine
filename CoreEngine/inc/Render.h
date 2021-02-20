#pragma once
#include <SDL.h>
#ifdef OPENGL_RENDERING
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>
#else
#include <SDL_image.h>
#include <SDL_ttf.h>
#endif
#include "WindowRender.h"
#include "SingletonHolder.h"

class Render: public SingletonHolder<Render>
{
private:
	friend class SingletonHolder<Render>;

	//<The surface contained by the window
	SDL_Surface* m_pScreenSurface = nullptr;

	//<The render window
	SDL_Renderer *m_pRenderer = nullptr;
#ifdef OPENGL_RENDERING

	//<Gl context
	void* m_glcontext = nullptr;
#endif
	bool initWindow(const char* title,
		int xpos,
		int ypos,
		const int& width,
		const int& height,
		int flags);
protected:
	Render();
	~Render();
public:
	bool init();
	void render();
	void clean();

	SDL_Renderer* getRenderer();

	uint32_t WindowPixelFormat();

	const SDL_PixelFormat* WindowSurfaceFormat();
};