#include "pch.h"
#include "Application.h"
#include <iostream>

int main(int argc, char* argv[])
{
	Application* app = Application::GetInstance();
	app->start();
	return 0;
}
