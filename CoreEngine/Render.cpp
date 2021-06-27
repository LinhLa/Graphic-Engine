#include "stdafx.h"
#include "Render.h"
#include "Configuration.h"
#include "Scene.h"

/* Draw a Gimpish background pattern to show transparency in the image */
static void draw_background(SDL_Renderer *renderer, int w, int h)
{
	SDL_Color col[2] = {
		{ 0x66, 0x66, 0x66, 0xff },
		{ 0x99, 0x99, 0x99, 0xff },
	};

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	int i, x, y;
	SDL_Rect rect;
	rect.w = 8;
	rect.h = 8;
	for (y = 0; y < h; y += rect.h) {
		for (x = 0; x < w; x += rect.w) {
			/* use an 8x8 checkerboard pattern */
			i = (((x ^ y) >> 3) & 1);
			SDL_SetRenderDrawColor(renderer, col[i].r, col[i].g, col[i].b, col[i].a);

			rect.x = x;
			rect.y = y;
			SDL_RenderFillRect(renderer, &rect);
		}
	}

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

Render::Render(){}

Render::~Render(){}

bool Render::initWindow(const char* title, int xpos, int ypos, const int& width, const int& height, int flags)
{
	//<Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		SDL_Log("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		_ASSERT(false);
	}

#ifdef OPENGL_RENDERING
	//Initialize OpenGL context
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, Configuration::GetInstance()->major_verion);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, Configuration::GetInstance()->minor_version);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE); //  Should be used in conjunction with the SDL_GL_CONTEXT_MAJOR_VERSION and SDL_GL_CONTEXT_MINOR_VERSION attributes

	//SDL_GL_SetAttribute(SDL_GL_RED_SIZE, Configuration::GetInstance()->r_size);
	//SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, Configuration::GetInstance()->g_size);
	//SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, Configuration::GetInstance()->b_size);
	//SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, Configuration::GetInstance()->depth_size);
	//SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, Configuration::GetInstance()->double_buffer);
#else
	//<Initialize SDL IMG
	if (IMG_Init(IMG_INIT_PNG) < 0)
	{
		SDL_Log("SDL IMG could not initialize! SDL_Error: %s\n", SDL_GetError());
		_ASSERT(false);
	}

	//Initialize the truetype font API.
	if (TTF_Init() < 0)
	{
		SDL_Log("%s", TTF_GetError());
		_ASSERT(false);
	}
#endif

	//<Create window
	WindowRender::GetInstance()->CreateWindow(title, xpos, ypos, width, height, flags);

#ifdef OPENGL_RENDERING
	//<Set view port
	glViewport(0, 0, width, height);

	// Create an OpenGL context associated with the window.
	m_glcontext = WindowRender::GetInstance()->CreateContext();

	//Since we want the latest features, we have to set glewExperimental to true
	glewExperimental = GL_TRUE;

	//Initialize GLEW
	GLenum glewError = glewInit();
	if (GLEW_OK != glewError)
	{
		SDL_Log("glewInit() failed: %s\n", glewGetErrorString(glewError));
		_ASSERT(false);
	}

	//Use Vsync
	if (SDL_GL_SetSwapInterval(1) < 0)
	{
		SDL_Log("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
#else
	//<Get window render
	m_pRenderer = WindowRender::GetInstance()->CreateRenderer(-1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);

	//<Set render blend mode
	if (0 < SDL_SetRenderDrawBlendMode(m_pRenderer, SDL_BLENDMODE_NONE))
	{
		SDL_Log("SDL_SetRenderDrawBlendMode() failed: %s\n", SDL_GetError());
		_ASSERT(false);
	}

	//<Set Render color
	if (-1 == SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255))
	{
		SDL_Log("SDL_SetRenderDrawColor() failed: %s\n", SDL_GetError());
		_ASSERT(false);
	}

	//<Get window surface
	m_pScreenSurface = WindowRender::GetInstance()->GetWindowSurface();

#endif
	WindowRender::GetInstance()->ShowWindow();
	return true;
}

void Render::clean()
{
#ifdef OPENGL_RENDERING
	//Destroy SDL_GLContext
	m_glcontext ? SDL_GL_DeleteContext(m_glcontext) : false;
#else
	//<Destroy render
	m_pRenderer ? SDL_DestroyRenderer(m_pRenderer) : false;
	m_pRenderer = NULL;

	//<Deallocate surface
	m_pScreenSurface ? SDL_FreeSurface(m_pScreenSurface) : false;
	m_pScreenSurface = NULL;
#endif
	//<Destroy window
	WindowRender::GetInstance()->DestroyWindow();

#ifndef OPENGL_RENDERING
	//<Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
#endif
	SDL_Quit();
}


void Render::render()
{
#ifdef OPENGL_RENDERING
	//<Set Clear color
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	//<Clear context
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#else
	//< clear the window
	SDL_RenderClear(m_pRenderer);
#endif
	//< render scene graph
	SDL_Event sdlevent;
	sdlevent.type = SDL_USEREVENT;
	sdlevent.user.code = 0x02;
	Scene::GetInstance()->onEvent(sdlevent);
#ifdef OPENGL_RENDERING
	//<Swap our buffer to display the current contents of buffer on screen
	WindowRender::GetInstance()->SwapWindow();
#else
	/* Update screen*/
	SDL_RenderPresent(m_pRenderer);
#endif
}

bool Render::init()
{
	Configuration *pConfig = Configuration::GetInstance();
	int screen_height = pConfig->height & 0xFFFFFFFF;
	int screen_width = pConfig->width & 0xFFFFFFFF;
	uint32_t flags = SDL_WINDOW_SHOWN /*| SDL_WINDOW_BORDERLESS*/ | SDL_WINDOW_RESIZABLE;
#ifdef OPENGL_RENDERING
	flags|= SDL_WINDOW_OPENGL;
#endif

	bool bResult = initWindow(Configuration::GetInstance()->app_name,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		screen_width,
		screen_height,
		flags);
	return bResult;
}

SDL_Renderer* Render::getRenderer()
{
	return m_pRenderer;
}

uint32_t Render::WindowPixelFormat()
{
	return m_pScreenSurface->format->format;
}

const SDL_PixelFormat* Render::WindowSurfaceFormat()
{
	return m_pScreenSurface->format;
}