#include "stdafx.h"
#include "WindowRender.h"
#include "Configuration.h"
#include "Scene.h"
#include "PropertyDefine.h"
#include "GLRenderManipulator.h"

WindowRender::WindowRender() {}

WindowRender::~WindowRender() {}

bool WindowRender::CreateWindow(const char* title, int xpos, int ypos, const int& width, const int& height, int flags)
{
	bool bResult = true;
	m_width = width;
	m_height = height;

	//<Create window
	m_pWindow = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
	if (!m_pWindow)
	{
		SDL_Log("SDL_CreateWindow() failed: %s\n", SDL_GetError());
		_ASSERT(false);
	}
	return bResult;
}

SDL_Window* WindowRender::getWindow()
{
	return m_pWindow;
}

#ifdef OPENGL_RENDERING
void* WindowRender::CreateContext()
{
	void* glcontext = nullptr;
	glcontext = SDL_GL_CreateContext(m_pWindow);
	if (NULL == glcontext)
	{
		SDL_Log("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
		_ASSERT(false);
	}
	return glcontext;
}

void WindowRender::SwapWindow()
{
	SDL_GL_SwapWindow(m_pWindow);
}
#else
SDL_Renderer* WindowRender::CreateRenderer(int index, uint32_t flags)
{
	m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, flags);
	if (!m_pRenderer) {
		SDL_Log("SDL_CreateRenderer() failed: %s\n", SDL_GetError());
		_ASSERT(false);
	}
	return m_pRenderer;
}

SDL_Surface* WindowRender::GetWindowSurface()
{
	SDL_Surface* pScreenSurface = SDL_GetWindowSurface(m_pWindow);
	if (!pScreenSurface)
	{
		SDL_Log("SDL_GetWindowSurface() failed: %s\n", SDL_GetError());
		_ASSERT(false);
	}
	return pScreenSurface;
}
#endif

void WindowRender::ShowWindow()
{
	SDL_ShowWindow(m_pWindow);
}

void WindowRender::DestroyWindow()
{
	if (m_pWindow)
	{
		SDL_DestroyWindow(m_pWindow);
		m_pWindow = nullptr;
	}
}

int WindowRender::getWidth() const
{
	return m_width;
}

int WindowRender::getHeight() const
{
	return m_height;
}

float WindowRender::scaleX() const
{
	return m_scaleX;
}
float WindowRender::scaleY() const
{
	return m_scaleY;
}

bool WindowRender::hasMouseFocus() const
{
	return m_bMouseFocus;
}

bool WindowRender::hasKeyboardFocus() const
{
	return m_bKeyboardFocus;
}

bool WindowRender::isMinimized() const
{
	auto flags = SDL_GetWindowFlags(m_pWindow);
	return (flags & SDL_WINDOW_MINIMIZED);
}

bool WindowRender::isMaximinzed() const
{
	auto flags = SDL_GetWindowFlags(m_pWindow);
	return (flags & SDL_WINDOW_MAXIMIZED);
}

bool WindowRender::isFullScreen() const
{
	auto flags = SDL_GetWindowFlags(m_pWindow);
	return (flags & SDL_WINDOW_FULLSCREEN);
}

void WindowRender::onWindowEvent(SDL_Event& e)
{
	//Window event occured
	if (e.type == SDL_WINDOWEVENT)
	{
		//Caption update flag
		bool updateCaption = false;
#ifdef OPENGL_RENDERING
		auto pRoot = Scene::GetInstance()->GetRoot();
		auto scale = pRoot->GetPropertyValue<glm::vec3>(SCALE_VECTOR);
#endif
		switch (e.window.event)
		{
			//Get new dimensions and repaint on window size change
		case SDL_WINDOWEVENT_SIZE_CHANGED:
			m_scaleX = float(e.window.data1) / Configuration::GetInstance()->width;
			m_scaleY = float(e.window.data2) / Configuration::GetInstance()->height;
			m_width = e.window.data1;
			m_height = e.window.data2;
#ifdef OPENGL_RENDERING
			GLRenderViewPortManipulator::clear();
			GLRenderViewPortManipulator::push(glm::i32vec4(0, 0, e.window.data1, e.window.data2));
			scale.x = m_scaleX;
			scale.y = m_scaleY;
			pRoot->SetPropertyValue<glm::vec3>(SCALE_VECTOR, scale);
#else
			SDL_RenderSetScale(m_pRenderer, m_scaleX, m_scaleY);
#endif
			break;

			//Repaint on exposure
		case SDL_WINDOWEVENT_EXPOSED:
			break;
			//Mouse entered window
		case SDL_WINDOWEVENT_ENTER:
			m_bMouseFocus = true;
			updateCaption = true;
			break;
			//Mouse left window
		case SDL_WINDOWEVENT_LEAVE:
			m_bMouseFocus = false;
			updateCaption = true;
			break;

			//Window has keyboard focus
		case SDL_WINDOWEVENT_FOCUS_GAINED:
			m_bKeyboardFocus = true;
			updateCaption = true;
			break;

			//Window lost keyboard focus
		case SDL_WINDOWEVENT_FOCUS_LOST:
			m_bKeyboardFocus = false;
			updateCaption = true;
			break;

			//Window restored
		case SDL_WINDOWEVENT_RESTORED:
			break;
		}
		//Update window caption with new data
		if (updateCaption)
		{
			//std::stringstream caption;
			//caption << "SDL Tutorial - MouseFocus:" << ( ( mMouseFocus ) ? "On" : "Off" ) << " KeyboardFocus:" << ( ( mKeyboardFocus ) ? "On" : "Off" );
			//SDL_SetWindowTitle( mWindow, caption.str().c_str() );
		}
	}
	//Enter exit full screen on return key
	else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN)
	{
		if (isFullScreen())
		{
			SDL_SetWindowFullscreen(m_pWindow, SDL_FALSE);
		}
		else
		{
			SDL_SetWindowFullscreen(m_pWindow, SDL_TRUE);
		}
	}
}
