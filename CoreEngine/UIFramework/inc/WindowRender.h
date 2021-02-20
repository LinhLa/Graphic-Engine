#pragma once
#include <SDL.h>
#ifdef OPENGL_RENDERING
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>
#endif
#include "SingletonHolder.h"
class Render;
class WindowRender: public SingletonHolder<WindowRender>
{
private:
	friend class SingletonHolder<WindowRender>;
	friend class Render;
	//<The window we'll be rendering to
	SDL_Window* m_pWindow = nullptr;
	//<The render window
	SDL_Renderer *m_pRenderer = nullptr;
	int m_width = 0;
	int m_height = 0;
	bool m_bMinimized = false;
	bool m_bFullScreen = false;
	bool m_bKeyboardFocus = false;
	bool m_bMouseFocus = false;

	float m_scaleX = 1.0F;
	float m_scaleY = 1.0F;
protected:
	WindowRender();
	~WindowRender();
	bool CreateWindow(const char* title,
		int xpos,
		int ypos,
		const int& width,
		const int& height,
		int flags);

	SDL_Window* getWindow();

	SDL_Renderer* CreateRenderer(int index, uint32_t flags);
	void* CreateContext();
	SDL_Surface* GetWindowSurface();

	void SwapWindow();
	void ShowWindow();
	void DestroyWindow();
public:

	int  getWidth() const;
	int  getHeight() const;
	float scaleX() const;
	float scaleY() const;

	bool hasMouseFocus() const;
	bool hasKeyboardFocus() const;
	bool isMinimized() const;

	void onWindowEvent(SDL_Event& e);
};
typedef std::shared_ptr<WindowRender> WindowRenderPtr;