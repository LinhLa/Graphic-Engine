#include "stdafx.h"
#include <stdio.h>
#include <string.h>

#include "Application.h"

#include <ServiceMgr.h>
#include <MemoryTracker.h>
#include <FrameService.h>
#include <ContentService.h>
#include <LogService.h>

#include "FpsTracking.h"

#include <Configuration.h>
#include <Render.h>
#include <Scene.h>

#include "deltatime.h"
#include <AnimationTimeLine.h>

#include "KeyMap.h"

#include "TaskPoolInstance.h"

#include "ImGuiShader.h"

const std::string Application::POST_LOAD_RESOURCE_SIGNAL = "POST_LOAD_RESOURCE_SIGNAL";
const std::string Application::PRE_RENDER_SIGNAL = "PRE_RENDER_SIGNAL";
const std::string Application::POST_RENDER_SIGNAL = "POST_RENDER_SIGNAL";
const std::string Application::KEY_INPUT_SIGNAL = "KEY_INPUT_SIGNAL";
const std::string Application::WINDOW_EVENT_SIGNAL = "WINDOW_EVENT_SIGNAL";

Application::Application()
{
	InitSignal();
}

Application::~Application() {}

void Application::InitSignal()
{
	ISignalPtr pSignal_loadResource = Signal<VoidType>::create();
	ISignalPtr pSignal_preRender = Signal<SDL_Event>::create();
	ISignalPtr pSignal_postRender = Signal<SDL_Event>::create();
	ISignalPtr pSignal_keyinput = Signal<SDL_Event>::create();
	ISignalPtr pSignal_windowEvent = Signal<SDL_Event>::create();

	AddSignal(POST_LOAD_RESOURCE_SIGNAL, pSignal_loadResource);
	AddSignal(PRE_RENDER_SIGNAL, pSignal_preRender);
	AddSignal(POST_RENDER_SIGNAL, pSignal_postRender);
	AddSignal(KEY_INPUT_SIGNAL, pSignal_keyinput);
	AddSignal(WINDOW_EVENT_SIGNAL, pSignal_windowEvent);
}

void Application::InitService()
{
	ServiceMgr *pServiceMgr = ServiceMgr::GetInstance();

	//m_service_directory[TEST_SERVICE_ID] = std::make_shared<TestService>();
	pServiceMgr->AttachService(new MemoryTracker);
	pServiceMgr->AttachService(new FrameService);
	pServiceMgr->AttachService(new LogService);
	pServiceMgr->AttachService(new ContentService);
}

void Application::init()
{
	InitService();

	///<load config
	Configuration::GetInstance()->loadConfig(CONFIG_FILE);

	///<Render init
	_ASSERT(Render::GetInstance()->init());

	///<load resource and signal
	Scene::GetInstance()->LoadResource();
	OnSignal<VoidType>(POST_LOAD_RESOURCE_SIGNAL, VoidType());

	///<init keymap
	KeyMap::GetInstance()->init();

	///<init Scene graph resource
	SDL_Event sdlevent;
	sdlevent.type = SDL_USEREVENT;
	sdlevent.user.code = 0x01;
	Scene::GetInstance()->onEvent(sdlevent);
}

void Application::start()
{
	///<start all service
	ServiceMgr::GetInstance()->StartService();

	uint32_t fps = Configuration::GetInstance()->fps & 0xFFFF;
	uint32_t delayTime = uint32_t(1000) / uint32_t(fps);

	///<FPS Timer start
	FpsTracking framePerSecond;
	framePerSecond.start();

	///<Delta timer start
	DeltaTime::GetInstance()->start();

	///<Render loop
	bool bQuit = false;

	while (!bQuit)
	{
		///<Event handler
		SDL_Event event;

		while (/*SDL_WaitEvent*/SDL_PollEvent(&event))///<Handle events on queue: use SDL_WaitEvent to optimize CPU performance
		{
			// without it you won't have keyboard input and other things
			ImGuiShader::GetInstance()->ProcessEvent(event);

			SDL_Event *sdlEvent;
			switch (event.type)
			{
			case SDL_WINDOWEVENT:
				///<on window event
				OnSignal<SDL_Event>(WINDOW_EVENT_SIGNAL, std::move(SDL_Event{ event }));
				WindowRender::GetInstance()->onWindowEvent(event);
				break;
			case SDL_MOUSEMOTION:
			case SDL_MOUSEWHEEL:
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
			case SDL_KEYDOWN:
			case SDL_KEYUP:
			case SDL_TEXTINPUT:
				//broadcast even to scene graph
				Scene::GetInstance()->onEvent(event);

				//sdlEvent = new SDL_Event(event);
				//ServiceMgr::GetInstance()->sendKeyInputMessage(Message(KEY_PRESS, reinterpret_cast<void*>(sdlEvent), [sdlEvent]{delete sdlEvent;}));

				///<on signal key input
				OnSignal<SDL_Event>(KEY_INPUT_SIGNAL, std::move(SDL_Event{ event }));
				break;
			case SDL_USEREVENT:
				Scene::GetInstance()->onEvent(event);
				break;
			case SDL_QUIT:
			{
				///<Set quit flag
				bQuit = true;
				Scene::GetInstance()->onEvent(event);
				break;
			}
			default:
				break;
			}
		}

		///<Break render loop on quit event
		if (bQuit)
		{
			break;
		}

		///<Run service dispatcher
		//TODO:: Move to service thread ServiceMgr::GetInstance()->dispatch();

		// Start the Dear ImGui frame
		ImGuiShader::GetInstance()->StartNewFrame();

		// Show ImGUI
		ImGuiShader::GetInstance()->Show();

		///<Play animation timeline
		AnimationTimeLine::GetInstance()->onPlay();

		///<Execute timer task
		TaskPoolInstance::GetInstance()->executeTaskList();

		///<on signal pre render
		OnSignal<SDL_Event>(PRE_RENDER_SIGNAL, std::move(SDL_Event{ event }));

		///<Render screen
		Render::GetInstance()->render();

		///<on signal post render
		OnSignal<SDL_Event>(POST_RENDER_SIGNAL, std::move(SDL_Event{ event }));

		///<Increate frame and calculate and correct fps
		framePerSecond.countFrames();

		///<Set Current counter
		DeltaTime::GetInstance()->now();
	}

	///<application is quit
	//ServiceMgr::GetInstance()->EndService();

	///<Clean resource
	Render::GetInstance()->clean();
}