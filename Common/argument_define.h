#pragma once
#include <memmgr.h>
#include <enum_event.h>
#include <SDL.h>
class ArgumentBase
{
protected:
	ArgumentBase() {}
public:
	virtual ~ArgumentBase() {}
};


struct NullType {};

struct RenderInitArgumentType : public ArgumentBase {
	enum { EventType = RENDER_EVENT_INIT, };
	RenderInitArgumentType() {};
};

struct RenderDrawArgumentType : public ArgumentBase {
	enum { EventType = RENDER_EVENT_DRAW, };
	SDL_Event m_event_data;

	RenderDrawArgumentType(const SDL_Event &event) : m_event_data(event)
	{}
	RenderDrawArgumentType() {}
	~RenderDrawArgumentType() {}

};

struct RenderCleanArgumentType : public ArgumentBase {
	enum { EventType = RENDER_EVENT_CLEAN, };
	RenderCleanArgumentType() {}
};

struct KeyInputArgumentType : public ArgumentBase {
	enum { EventType = RENDER_EVENT_KEY_INPUT, };
	SDL_Event m_event_data;
	KeyInputArgumentType(const SDL_Event &event) : m_event_data(event)
	{}
	KeyInputArgumentType() {}
	~KeyInputArgumentType() {}
};

struct RenderLoopArgumentType : public ArgumentBase {
	enum { EventType = RENDER_EVENT_LOOP, };
};
