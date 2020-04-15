#include "stdafx.h"
#include "ServiceMgr.h"

int main(int argc, char* argv[])
{
	ServiceMgr::GetInstance()->InitService();
	ServiceMgr::GetInstance()->StartService();
    return 0;
}

