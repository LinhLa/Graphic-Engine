#include "stdafx.h"
#include <iostream>
#include <conio.h>
#include "ServiceMgr.h"
#include "TestService.h"

int main(int argc, char* argv[])
{
	ServiceMgr::GetInstance()->AttachService(new TestService);
	ServiceMgr::GetInstance()->StartService();

	ServiceMgr::GetInstance()->sendMessage({ TEST_MEMORY, 0ULL });
	ServiceMgr::GetInstance()->dispatch();
	ServiceMgr::GetInstance()->EndService();
	_getch();

    return 0;
}

