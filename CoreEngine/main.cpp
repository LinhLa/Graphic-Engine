// SDLApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include "Application.h"

#include "Player.h"
#include "Cube3D.h"

#ifdef main	// Disable marco SDL2
#undef main
#endif

int main(int argc, char* argv[])
{
	Application* app = Application::GetInstance();

	//auto pPlayer = Player::create();
	//app->bind(Application::POST_LOAD_RESOURCE_SIGNAL, pPlayer.get(), &Player::init);
	//app->bind(Application::KEY_INPUT_SIGNAL, pPlayer.get(), &Player::onKeyInputEvent);

	auto pCube3D = Cube3D::create();
	app->bind(Application::POST_LOAD_RESOURCE_SIGNAL, pCube3D.get(), &Cube3D::init);
	app->bind(Application::KEY_INPUT_SIGNAL, pCube3D.get(), &Cube3D::onKeyInputEvent);
	app->bind(Application::PRE_RENDER_SIGNAL, pCube3D.get(), &Cube3D::onRenderEvent);
	app->init();
	app->start();

	return 0;
}