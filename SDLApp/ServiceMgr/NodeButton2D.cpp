#include "stdafx.h"
#include <base_define.h>
#include <NodeButton2D.h>

NodeButton2D::NodeButton2D(){}

NodeButton2D::~NodeButton2D() {}

void NodeButton2D::onKeyInputEvent(KeyInputArgumentType &arg)
{
	SDL_event &event = &arg.m_event_data;
	//Get mouse position
	int x, y;
	SDL_GetMouseState( &x, &y );
        //Check if mouse is in button
	bool inside = true;

     //Mouse is left of the button
	if( x < m_position.x )
	{
		inside = false;
	}
        //Mouse is right of the button
	else if( x > m_position.x + m_position )
	{
		inside = false;
	}
     //Mouse above the button
	else if( y < m_position.y )
	{
		inside = false;
	}
    //Mouse below the button
	else if( y > m_position.y + m_position.h )
	{
		inside = false;
	}

	 //Mouse is outside button
	if( !inside )
	{
		m_CurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
		return;
	}
	else
	{
        //Mouse is inside button
		switch( event->type )
		{
			case SDL_MOUSEMOTION:
			m_CurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
			break;
			case SDL_MOUSEMOTION:
			m_CurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
			break;
			case SDL_MOUSEBUTTONDOWN:
			m_CurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
			break;
			case SDL_MOUSEBUTTONUP:
			m_CurrentSprite = BUTTON_SPRITE_MOUSE_UP;
			break;
			default:
			break;
		}
	}
}

void NodeButton2D::onInit(RenderInitArgumentType &arg)
{
	//<create resource path
	std::string resource_path = m_pSceneConfig->GetAssetPath() + m_resource_name;

	//<Init backgournd texture and text texture
	m_pBackground_texture->onInit(arg);
	m_pText_texture->onInit(arg);
}

void NodeButton2D::onDraw(RenderDrawArgumentType &arg)
{
	if (!m_bVisible)
	{
		return;
	}
	//<draw backgournd texture 1st and text texture 2nd
	m_pBackground_texture->onDraw(arg);
	m_pText_texture->onDraw(arg);
}

void NodeButton2D::onClean(RenderCleanArgumentType &arg)
{
	//<clean backgournd texture and text texture
	m_pBackground_texture->onClean(arg);
	m_pText_texture->onClean(arg);
}

void NodeButton2D::onLoop(RenderLoopArgumentType& arg) {}