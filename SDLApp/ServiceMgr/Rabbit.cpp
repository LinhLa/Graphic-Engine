#include "stdafx.h"
#include "Rabbit.h"
#include <ServiceMgr.h>
#include <SceneService.h>
#include <ResourceAcquire.h>
Rabbit::Rabbit(){}

Rabbit::~Rabbit(){}

void Rabbit::onKeyInputEvent(KeyInputArgumentType &arg)
{
	SDL_Rect dest_rect = m_pRabbit->GetDestRect();
	SDL_Rect src_rect = m_pRabbit->GetSrcRect();

	const Uint8 *keyboard_state_array = SDL_GetKeyboardState(NULL);
	if (keyboard_state_array[SDL_SCANCODE_LEFT])
	{
		dest_rect.x -= 20;
	}

	if (keyboard_state_array[SDL_SCANCODE_RIGHT])
	{
		dest_rect.x += 20;
	}

	//<Play animation
	src_rect.x = 200 * int(((SDL_GetTicks() / 100) % 8));

	//<update rapit source and destination rectangular
	m_pRabbit->SetSrcRect(src_rect);
	m_pRabbit->SetDestRect(dest_rect);
}
void Rabbit::InitResource()
{
	//<
}

void Rabbit::AcquireResource()
{
	//<Get resource for node m_pRabbit
	QuerryResourceAsync(m_pRabbit, NODE_2D_IMAGE_TYPE, RAPID_SPRITE_SHEET_ASSET);
}

void Rabbit::setVisible(bool visible)
{
	m_pRabbit->setVisible(visible);
	m_pRextStatus->setVisible(visible);
}