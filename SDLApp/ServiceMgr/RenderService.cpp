#include "stdafx.h"
#include <RenderService.h>
#include <base_define.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <Timer.h>
#include <utils.h>


/**
 * @brief      This class describes a fps tracking.
 */
class CFpsTracking
{
private:
	Timer 		m_fpsTimer;
	float 		m_avgFPS = 0.0F;
	uint32_t 	m_countedFrames = 0;
public:
	CFpsTracking(){}
	~CFpsTracking(){}

	void 		start(){m_fpsTimer.start();}
	uint32_t 	getFps(){ return avgFPS;}

	void 		countFrames()
	{
		++m_countedFrames;
		 avgFPS = countedFrames / fpsTimer.getSeconds();
	}
};


/**
 * @brief      This class describes a frame rate capping.
 */
class CFrameRateCapping
{
private:
	//<Capping frame rate
	uint32_t m_delayTime = 0;
	uint32_t m_FrameStart = 0;
	uint32_t m_FrameTime = 0;
public:
	CFrameRateCapping(){}
	~CFrameRateCapping(){}

	void Capping()
	{
		//<Calculate Frame Time
		m_FrameTime = SDL_GetTicks() - m_FrameStart;

		if (m_FrameTime < m_delayTime)
		{
			SDL_Delay(m_delayTime - m_FrameTime);
		}
	}

	void FrameStart(){m_FrameStart = SDL_GetTicks();}
};

/* Draw a Gimpish background pattern to show transparency in the image */
static void draw_background(SDL_Renderer *renderer, int w, int h)
{
	SDL_Color col[2] = {
		{ 0x66, 0x66, 0x66, 0xff },
		{ 0x99, 0x99, 0x99, 0xff },
	};
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
}

RenderService::RenderService()
{

}

RenderService::~RenderService()
{
}


void RenderService::onStartService()
{
	//<Add trigger event to service manager
	m_pServiceMgr->registerOnEvent(this, CONFIG_UPDATE_SCREEN_HEIGHT);
	m_pServiceMgr->registerOnEvent(this, CONFIG_UPDATE_SCREEN_WIDTH);
	m_pServiceMgr->registerOnEvent(this, CONFIG_LOADED);
	m_pServiceMgr->registerOnEvent(this, RENDER_UNREGISTER_CALLBACK);
	m_pServiceMgr->registerOnEvent(this, RENDER_REGISTER_CALLBACK);
	m_pServiceMgr->registerOnEvent(this, SCENE_LOAD_RESOURCE_DONE);
	m_pServiceMgr->registerOnEvent(this, CONFIG_UPDATE_FPS);
}

void RenderService::onEndService()
{
	//<Remove trigger event to service manager
	m_pServiceMgr->unregisterOnEvent(this, CONFIG_UPDATE_SCREEN_HEIGHT);
	m_pServiceMgr->unregisterOnEvent(this, CONFIG_UPDATE_SCREEN_WIDTH);
	m_pServiceMgr->unregisterOnEvent(this, CONFIG_LOADED);
	m_pServiceMgr->unregisterOnEvent(this, RENDER_UNREGISTER_CALLBACK);
	m_pServiceMgr->unregisterOnEvent(this, RENDER_REGISTER_CALLBACK);
	m_pServiceMgr->unregisterOnEvent(this, SCENE_LOAD_RESOURCE_DONE);
	m_pServiceMgr->unregisterOnEvent(this, CONFIG_UPDATE_FPS);
}


bool RenderService::initWindow(const char* title, int xpos, int ypos, const int& width, const int& height, int flags)
{
	//<Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0 || IMG_Init(IMG_INIT_PNG) < 0)
	{
		SDL_Log("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		_ASSERT(false);
	}

	//Initialize the truetype font API.
	if (TTF_Init() < 0)
	{
		SDL_Log("%s", TTF_GetError());
		_ASSERT(false);
	}

	//<Create window
	m_pWindow = SDL_CreateWindow(title,
		xpos,
		ypos,
		width, height, flags);

	if (!m_pWindow)
	{
		SDL_Log("SDL_CreateWindow() failed: %s\n", SDL_GetError());
		_ASSERT(false);
	}

	//<Get window render
	m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
	if (!m_pRenderer) {
		SDL_Log("SDL_CreateRenderer() failed: %s\n", SDL_GetError());
		_ASSERT(false);
	}

	//<Set Render color
	if (-1 == SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255))
	{
		SDL_Log("SDL_SetRenderDrawColor() failed: %s\n", SDL_GetError());
		_ASSERT(false);
	}

	//<Get window surface
	m_pScreenSurface = SDL_GetWindowSurface(m_pWindow);
	if (!m_pScreenSurface)
	{
		SDL_Log("SDL_GetWindowSurface() failed: %s\n", SDL_GetError());
		_ASSERT(false);
	}

	//<Load image to texture and show window
	m_pBackgroundTexture = std::make_shared<Texture>();
	_ASSERT(m_pBackgroundTexture->loadImage("./Asset/" BACK_GROUND_ASSERT, m_pRenderer));

	SDL_ShowWindow(m_pWindow);

	return true;
}

void RenderService::clean()
{
	//<Destroy background texture
	m_pBackgroundTexture.reset();

	//<Destroy render
	m_pRenderer ? SDL_DestroyRenderer(m_pRenderer) : false;
	m_pRenderer = NULL;

	//<Deallocate surface
	m_pScreenSurface ? SDL_FreeSurface(m_pScreenSurface) : false;
	m_pScreenSurface = NULL;

	//<Destroy window
	m_pWindow ? SDL_DestroyWindow(m_pWindow) : false;
	m_pWindow = NULL;

	//<Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}


void RenderService::render()
{
	static CFrameRateCapping frameCapping;
	frameCapping.Capping();

	//< clear the window
	SDL_RenderClear(m_pRenderer);
	onEvent(RenderDrawArgumentType(m_event));

	/* Update screen*/
	SDL_RenderPresent(m_pRenderer);

	//<Set Start Point
	frameCapping.FrameStart();
}

void RenderService::processUserEvent()
{
	switch (m_event.user.code)
	{
	case RENDER_EVENT_INIT:
		onEvent(RenderInitArgumentType());
		break;
	case RENDER_EVENT_CLEAN:
		onEvent(RenderCleanArgumentType());
		break;
	default:
		break;
	}
}

void RenderService::processing()
{
	if (!initWindow("Sample", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_screen_width, m_screen_height, SDL_WINDOW_SHOWN))
	{
		//<<Nothing to do
		_ASSERT(false);
	}
	else
	{
		//<Send message to update render instance
		uint64_t address[2] = { reinterpret_cast<uint64_t>(m_pRenderer), reinterpret_cast<uint64_t>(m_pScreenSurface)};
		m_pServiceMgr->sendMessage({ RENDER_INSTANCE_UPDATE, message_data(address, sizeof(address)) });
		//<Wait for loading resource from scene service
		m_pServiceMgr->waitForMessage(SCENE_INIT_DONE);

		//<Init Graphic resource
		onEvent(RenderInitArgumentType());

		//<FPS Timer start
		CFpsTracking framePerSecond;
		framePerSecond.start();

		//<Handle events on queue: use SDL_WaitEvent to optimize CPU performance
		while (SDL_WaitEvent(&m_event) != 0)
		{
			if (SDL_WINDOWEVENT == m_event.type)
			{
				render();
				continue;
			}

			switch (m_event.type)
			{
			case SDL_QUIT://<User requests quit
			{
				//<Set quit flag
				m_bQuit.store(true, std::memory_order_release);
				break;
			}
			case SDL_KEYDOWN:	//<User presses a key
			{
				//<process key down event
				onEvent(KeyInputArgumentType(m_event));
				break;
			}
			case SDL_USEREVENT:	//<User event
			{
				processUserEvent();
				break;
			}
			default:
				break;
			}

			//<if application is quit
			if (true == m_bQuit.load(std::memory_order_acquire))
			{
				//<Clean resource
				onEvent(RenderCleanArgumentType());

				clean();

				return;
			}
			else
			{
				//<render screen
				render();
				//Increate frame and calculate and correct fps
				framePerSecond.countFrames();
			}
		}
	}
}

void RenderService::onMessageReceive(const Message &msg)
{
	//Message &msg = *static_cast<Message*>(pData);
	switch (msg.m_id)
	{
	case CONFIG_UPDATE_SCREEN_HEIGHT:
		m_screen_height = msg.m_iValue & 0xFFFFFFFF;
		break;
	case CONFIG_UPDATE_SCREEN_WIDTH:
		m_screen_width = msg.m_iValue & 0xFFFFFFFF;
		break;
	case CONFIG_LOADED:
		m_pServiceMgr->sendMessage({RENDER_CONFIG_LOADED, 0ULL});
		break;
	case RENDER_REGISTER_CALLBACK:
		_ASSERT(msg.m_msg_function.m_pFunct);
		m_callback_list[msg.m_msg_function.m_evenType].push_back(msg.m_msg_function.m_pFunct);
		break;
	case RENDER_UNREGISTER_CALLBACK:
		_ASSERT(msg.m_msg_function.m_pFunct);
		m_callback_list[msg.m_msg_function.m_evenType].remove(msg.m_msg_function.m_pFunct);
		break;
	case CONFIG_UPDATE_FPS:
		m_fps = msg.m_uValue & 0xFF;
		m_delayTime = uint32_t(1000) / uint32_t(m_fps);
		break;
	default:
		break;
	}
}
