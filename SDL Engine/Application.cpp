#include "pch.h"
#include "Application.h"
#include <stdio.h>
#include <string.h>

Application::Application()
{
}
Application::~Application() {}

void Application::start()
{
	///<get service manager instance
	ServiceMgr *pServiceMgr = ServiceMgr::GetInstance();

	///<Init service manager
	pServiceMgr->InitService();

	///<start all service
	pServiceMgr->StartService();
}