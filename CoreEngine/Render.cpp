	#include "stdafx.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "GLRenderManipulator.h"

#include "Render.h"
#include "Configuration.h"
#include "Scene.h"

static void SDL_CHECK(int result, std::string comment = "")
{
	if (result < 0)
	{
		SDL_Log("%s SDL_Error: % s\n", comment.c_str(), SDL_GetError());
		_ASSERT(false);
	}
}

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
	SDL_CHECK(SDL_Init(SDL_INIT_VIDEO), "SDL could not initialize!");
	auto pConfig = Configuration::GetInstance();
	auto pWindowRender = WindowRender::GetInstance();
#ifdef OPENGL_RENDERING
	//Initialize OpenGL context
	SDL_CHECK(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, pConfig->major_verion));
	SDL_CHECK(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, pConfig->minor_version));
	SDL_CHECK(SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE)); //  Should be used in conjunction with the SDL_GL_CONTEXT_MAJOR_VERSION and SDL_GL_CONTEXT_MINOR_VERSION attributes

	SDL_CHECK(SDL_GL_SetAttribute(SDL_GL_RED_SIZE, pConfig->r_size));
	SDL_CHECK(SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, pConfig->g_size));
	SDL_CHECK(SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, pConfig->b_size));
	SDL_CHECK(SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, pConfig->a_size));
	SDL_CHECK(SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, pConfig->depth_size));
	SDL_CHECK(SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, pConfig->double_buffer ? 1 : 0));
	SDL_CHECK(SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, pConfig->stencil_size));

	if (pConfig->msaa)
	{
		SDL_CHECK(SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, pConfig->msaa_buffer));
		SDL_CHECK(SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, pConfig->msaa_sample));
	}
#else
	//<Initialize SDL IMG
	SDL_CHECK(IMG_Init(IMG_INIT_PNG, "SDL IMG could not initialize!");

	//Initialize the truetype font API.
	SDL_CHECK(TTF_Init());
#endif

	//<Create window
	pWindowRender->CreateWindow(title, xpos, ypos, width, height, flags);

#ifdef OPENGL_RENDERING
	
	// Create an OpenGL context and associated to window.
	m_glcontext = pWindowRender->CreateContext();
	SDL_GL_MakeCurrent(pWindowRender->getWindow(), m_glcontext);

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
	if (pConfig->vsync)
	{
		SDL_CHECK(SDL_GL_SetSwapInterval(1), "Warning: Unable to set VSync!");
	}

	SDL_CHECK(SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"), "Warning: Linear texture filtering not enabled :");
	SDL_CHECK(SDL_SetHint(SDL_HINT_MOUSE_RELATIVE_SCALING, "1"), "Warning: Mouse relative scaling not enabled :");

	//<Print some GL infomation
	SDL_Log("%s", glGetString(GL_RENDERER));
	SDL_Log("%s", glGetString(GL_VENDOR));
	SDL_Log("%s", glGetString(GL_SHADING_LANGUAGE_VERSION));

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(pWindowRender->getWindow(), m_glcontext);	
    ImGui_ImplOpenGL3_Init("#version 330");

	glEnable(GL_SCISSOR_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_MULTISAMPLE);

	//<Init view port
	GLRenderViewPortManipulator::push(glm::ivec4(0, 0, width, height));

	//<Init fbo
	GLRenderFBOManipulator::push(0U);

#else
	//<Get window render
	m_pRenderer = pWindowRender->CreateRenderer(-1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);

	//<Set render blend mode
	SDL_CHECK(SDL_SetRenderDrawBlendMode(m_pRenderer, SDL_BLENDMODE_NONE), "SDL_SetRenderDrawBlendMode() failed");

	//<Set Render color
	SDL_CHECK(SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255), "SDL_SetRenderDrawColor() failed:");

	//<Get window surface
	m_pScreenSurface = pWindowRender->GetWindowSurface();

#endif
	pWindowRender->ShowWindow();
	return true;
}

void Render::clean()
{
#ifdef OPENGL_RENDERING
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

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

void Render::drawScene()
{
#ifdef OPENGL_RENDERING
	GLRenderContextManipulator context_manip;
	context_manip.push(GLRenderMultisample::create("Multisample whole screen"));
	context_manip.excute();
#endif
	//< render scene graph
	SDL_Event sdlevent;
	sdlevent.type = SDL_USEREVENT;
	sdlevent.user.code = RENDER;
	Scene::GetInstance()->onEvent(sdlevent);
#ifdef OPENGL_RENDERING
	context_manip.finish();

	//post processing effect
	//auto context = GLRenderEffect::create(
	//	std::string("post processing"), 
	//	std::initializer_list<IKernelPtr>{
	//		InversionKernel::create(),
	//		BlurKernel::create(),
	//		GrayscaleKernel::create()
	//});
	//context_manip.push(context);
	//context_manip.excuteAll();
#endif
}

void Render::render()
{
#ifdef OPENGL_RENDERING
	//<bind to default frambuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0U);

	//<Set Clear color
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	//<Clear context
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
#else
	//< clear the window
	SDL_RenderClear(m_pRenderer);
#endif
	//< render scene graph
	drawScene();

#ifdef OPENGL_RENDERING

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	//<Swap buffer to display the current contents of buffer on screen
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
	uint32_t flags = SDL_WINDOW_SHOWN /*| SDL_WINDOW_BORDERLESS*/ | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;
#ifdef OPENGL_RENDERING
	flags|= SDL_WINDOW_OPENGL;
#endif

	bool bResult = initWindow(pConfig->app_name,
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