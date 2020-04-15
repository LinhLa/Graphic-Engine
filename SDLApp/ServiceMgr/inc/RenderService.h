#pragma once
#include <base_define.h>
#include <type_define.h>
#include <IService.h>
#include <ServiceMgr.h>
#include <SceneService.h>
#include <Texture.h>
#include <atomic>

class RenderService : public IService, public IMessageHandler<RENDER_EVENT_INIT, RENDER_EVENT_END - 1>
{
private:
	friend class ServiceMgr;

	TexturePtr	m_pBackgroundTexture = nullptr;

	int m_screen_width= -1;
	int m_screen_height = -1;

	//<The window we'll be rendering to
	SDL_Window* m_pWindow = nullptr;

	//<The surface contained by the window
	SDL_Surface* m_pScreenSurface = nullptr;

	//<The render window
	SDL_Renderer *m_pRenderer = nullptr;

	//<Main loop flag
	std::atomic_bool m_bQuit = false;

	//<Event handler
	SDL_Event m_event;

	bool initWindow(const char* title,
		int xpos,
		int ypos,
		const int& width,
		const int& height,
		int flags);

	void clean();
	void render();

	void processUserEvent();

public:
	RenderService();
	~RenderService();

	void processing();

	void onStartService() override;
	void onEndService() override;
	void onMessageReceive(const Message&) override;
};

typedef std::shared_ptr<RenderService> RenderServicePtr;