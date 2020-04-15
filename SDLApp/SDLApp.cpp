// SDLApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Application.h"
#include <iostream>

#ifdef main	// Disable marco SDL2
#undef main
#endif


int main(int argc, char* argv[])
{
	Application* app = Application::GetInstance();
	app->start();
	return 0;
}

