#include "stdafx.h"
#include "KeyInputSignalMethod.h"
#include "SignalDefine.h"

KeyInputSignalMethod::KeyInputSignalMethod(SignalTable* pSignalTable):m_pSignalTable(pSignalTable)
{
	if (!m_pSignalTable)
	{
		throw std::logic_error("invalid_argument");
	}
}

KeyInputSignalMethod::~KeyInputSignalMethod() {}

void KeyInputSignalMethod::init()
{
	m_pSignalTable->AddSignal(MOUSE_OUT_SIGNAL, Signal<SDL_Point>::create());
	m_pSignalTable->AddSignal(MOUSE_OVER_MOTION_SIGNAL, Signal<SDL_Point>::create());
	m_pSignalTable->AddSignal(MOUSE_UP_SIGNAL, Signal<SDL_Point>::create());
	m_pSignalTable->AddSignal(MOUSE_DOWN_SIGNAL, Signal<SDL_Point>::create());
	m_pSignalTable->AddSignal(MOUSE_RELEASE_SIGNAL, Signal<SDL_Point>::create());
	m_pSignalTable->AddSignal(MOUSE_DRAG_SIGNAL, Signal<SDL_Point>::create());
	m_pSignalTable->AddSignal(KEY_PRESS_SIGNAL, Signal<uint32_t, uint32_t>::create());
}
