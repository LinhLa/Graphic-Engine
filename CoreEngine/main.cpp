// SDLApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include "Application.h"

#include "Player.h"
#include "BackGround.h"
#include "PlayerButton.h"
#include "FullSizeSymbol.h"
#include "Speaker.h"
#include "ProgressBar.h"
#include "TimeSlide.h"

#ifdef main	// Disable marco SDL2
#undef main
#endif

int main(int argc, char* argv[])
{
	Application* app = Application::GetInstance();

	PlayerPtr pPlayer = Player::create();
	pPlayer->addComponent(PlayerButton::create(pPlayer));
	pPlayer->addComponent(BackGround::create(pPlayer));
	pPlayer->addComponent(PlayerButton::create(pPlayer));
	pPlayer->addComponent(TimeSlide::create(pPlayer));
	pPlayer->addComponent(ProgressBar::create(pPlayer));
	pPlayer->addComponent(Speaker::create(pPlayer));
	pPlayer->addComponent(FullSizeSymbol::create(pPlayer));

	app->bind(Application::POST_LOAD_RESOURCE_SIGNAL, pPlayer.get(), &Player::init);
	app->bind(Application::KEY_INPUT_SIGNAL, pPlayer.get(), &Player::onKeyInputEvent);

	app->init();
	app->start();

	return 0;
}